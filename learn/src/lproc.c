/*
The main purpose is to show the use of multiple Lua states in a multithreading contex.

The library identifies channels by strings and uses them to match senders and receivers.
A send operation can send any number of string values, which are returned by the matching receive operation.
All communication is synchronous: a process sending a message to a channel blocks until
there is a process receiving from this channel, while a process receiving from a channel
blocks until there is a process sending to it.

The implementation of lproc is also simple. It uses two circular double-linked lists,
one for processes waiting to send a message and another for processes waiting to receive a message.
It uses a single mutex to control access to these lists. Each process has an associated condition variable.
When a process wants to send a message to a channel, it traverses the receiving list looking for a process waiting
for that channel. If it finds one, it removes the process from the waiting list, moves the message's values
from itself to the found process, and signals the other process. Otherwise, it inserts itself into the sending
list and waits on its condition variable. To receive a message, it does a symmetrical operation.
*/

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include <pthread.h>
#include <string.h>

typedef struct Proc
{
    /* The Lua state used by the process */
    lua_State *L;

    /* The C thread that runs the process */
    pthread_t thread;

    /*
    The condition variable that the thread uses to block
    itself when it has to wait for a matching send/receive.
    */
    pthread_cond_t cond;

    /* The channel that the process is waiting, if any */
    const char *channel;

    /* They are used to link the process structure into a waiting list. */
    struct Proc *previous, *next;
} Proc;

int luaopen_lproc(lua_State *L);

/* The waiting send list */
static Proc *wait_sends = NULL;

/* The waiting receive list */
static Proc *wait_receives = NULL;

/* The single mutex to control access to waiting lists */
static pthread_mutex_t kernel_access = PTHREAD_MUTEX_INITIALIZER;

/*
Each process needs a Proc structure, and it needs to access this structure whenever its script calls send
or receive. The only parameter that these functions receive is the process's Lua state; therefore, each
process should store its Proc structure inside its Lua state. In our implementation, each state keeps its
corresponding Proc structure as a full userdata in the registry, associated with the key "_SELF". The
auxiliary function get_self retrieves the Proc structure associated with a given state.
*/
static Proc *get_self(lua_State *L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "_SELF");
    Proc *p = (Proc *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return p;
}

/*
Moves values from a sender process to a receiver process.

It moves to the receiver all values in the sender stack but the first,
which will be the channel. Note that, as we are pushing an arbitrary number of elements,
we have to check for stack space.
*/
static void move_values(lua_State *send, lua_State *rec)
{
    int n = lua_gettop(send);

    /* check rec space */
    luaL_checkstack(rec, n, "too many results");

    /* move values to receiver */
    for (int i = 2; i <= n; i++)
    {
        lua_pushstring(rec, lua_tostring(send, i));
    }
}

/*
If it finds one, it removes the process from the list and returns it;
otherwise, the function returns NULL
*/
static Proc *search_match(const char *channel, Proc **list)
{
    for (Proc *node = *list; node != NULL; node = node->next)
    {
        if (strcmp(channel, node->channel) == 0) /* match */
        {
            if (*list == node)                                    /* first element */
                *list = (node->next == node) ? NULL : node->next; /* whether only one element */

            node->previous->next = node->next;
            node->next->previous = node->previous;
            return node;
        }
    }
    return NULL;
}

/*
The process links itself at the end of the appropriate waiting list and
waits until another process matches with it and wakes it up. (The loop
around pthread_cond_wait handles the spurious wakeups allowed in POSIX threads.)
When a process wakes up another, it sets the other process's field channel to NULL.
So, if p->channel is not NULL, it means that nobody matched process p, so it has to keep waiting
*/
static void wait_on_list(lua_State *L, const char *channel, Proc **list)
{
    Proc *p = get_self(L);

    /* link itself at the end of the list */
    if (*list == NULL) /* empty list */
    {
        *list = p;
        p->previous = p->next = p;
    }
    else
    {
        p->previous = (*list)->previous;
        p->next = *list;
        p->previous->next = p->next->previous = p;
    }
    p->channel = channel;

    /* wait on its condition variable */
    do
    {
        pthread_cond_wait(&p->cond, &kernel_access);
    } while (p->channel);
}

/*
Define thread body.

First, it opens the standard Lua libraries and the lproc library. Second, it creates and initializes its own
control block. Then, it calls its main chunk. Finally, it destroys its condition variable and closes its Lua state.

Note the use of luaL_requiref to open the lproc library. This function is somewhat equivalent to
require but, instead of searching for a loader, it uses the given function (luaopen_lproc, in our
case) to open the library. After calling the open function, luaL_requiref registers the result in the
package.loaded table, so that future calls to require the library will not try to open it again. With true
as its last parameter, it also registers the library in the corresponding global variable (lproc, in our case).
*/
static void *lproc_thread(void *arg)
{
    lua_State *L = (lua_State *)arg;
    luaL_openlibs(L);
    luaL_requiref(L, "lproc", luaopen_lproc, 1);
    lua_pop(L, 1); /* remove result from previous call */

    Proc *self = (Proc *)lua_newuserdata(L, sizeof(Proc));
    lua_setfield(L, LUA_REGISTRYINDEX, "_SELF");
    self->L = L;
    self->thread = pthread_self();
    self->channel = NULL;
    pthread_cond_init(&self->cond, NULL);

    if (lua_pcall(L, 0, 0, 0) != 0)
        fprintf(stderr, "thread error:%s\n", lua_tostring(L, -1));

    pthread_cond_destroy(&get_self(L)->cond);
    lua_close(L);
    return NULL;
}

/*
Start a new process to run the given chunk (a string).
The library implements a Lua process as a C thread plus its associated Lua state.

This function creates a new Lua state L1 and compiles the given chunk in this new state. In case of error,
it signals the error to the original state L. Then it creates a new thread (using pthread_create) with
body lproc_thread, passing the new state L1 as the argument to the body. The call to pthread_detach
tells the system that we will not want any final answer from this thread.
*/
static int lproc_start(lua_State *L)
{
    const char *chunk = luaL_checkstring(L, 1);

    lua_State *L1 = luaL_newstate();
    if (L1 == NULL)
        luaL_error(L, "unable to create new state\n");

    if (luaL_loadstring(L1, chunk) != 0)
        luaL_error(L, "error in thread body:%s\n", lua_tostring(L, -1));

    pthread_t thread;
    if (pthread_create(&thread, NULL, lproc_thread, L1) != 0)
        luaL_error(L, "unable to create new thread\n");

    pthread_detach(thread);
    return 0;
}

/*
sends all given values (which should be strings) to the given channel identified by its name,
also a string. (The exercises will ask you to add support for sending other types.)

The function starts getting the channel. Then it locks the mutex and searches for a matching receiver.
If it finds one, it moves its values to this receiver, marks the receiver as ready, and wakes it up.
Otherwise, it puts itself on wait. When it finishes the operation,
it unlocks the mutex and returns with no values to Lua
*/
static int lproc_send(lua_State *L)
{
    const char *channel = luaL_checkstring(L, 1);

    pthread_mutex_lock(&kernel_access);
    Proc *p = search_match(channel, &wait_receives);
    if (p) /* found a matching receiver? */
    {
        move_values(L, p->L);          /* move values to receiver */
        p->channel = NULL;             /* mark receiver as not waiting */
        pthread_cond_signal(&p->cond); /* wake it up */
    }
    else
    {
        wait_on_list(L, channel, &wait_sends);
    }
    pthread_mutex_unlock(&kernel_access);

    return 0;
}

/*
receives the values sent to the given channel
*/
static int lproc_receive(lua_State *L)
{
    const char *channel = luaL_checkstring(L, 1);
    lua_settop(L, 1);

    pthread_mutex_lock(&kernel_access);
    Proc *p = search_match(channel, &wait_sends);
    if (p) /* found a matching sender? */
    {
        move_values(p->L, L);          /* get values from sender */
        p->channel = NULL;             /* mark sender as not waiting */
        pthread_cond_signal(&p->cond); /* wake it up */
    }
    else
    {
        wait_on_list(L, channel, &wait_receives);
    }
    pthread_mutex_unlock(&kernel_access);

    /* return all stack values except the channel */
    return lua_gettop(L) - 1;
}

/*
finishes a process. Only the main process needs this function.
If this process ends without calling lproc.exit, the whole program terminates,
without waiting for the end of the other processes
*/
static int lproc_exit(lua_State *L)
{
    pthread_exit(NULL);
    return 0;
}

static const struct luaL_Reg lproc_funcs[] = {
    {"start", lproc_start}, {"send", lproc_send}, {"receive", lproc_receive}, {"exit", lproc_exit}, {NULL, NULL}};

int luaopen_lproc(lua_State *L)
{
    luaL_newlib(L, lproc_funcs);
    return 1;
}
