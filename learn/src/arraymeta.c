/*
array.c改进版本:
将userdata和metatable关联起来，
可通过判断userdata关联的metatable来判断userdata是否为预期类型，
通过这种方法可检测到用户传递的userdata类型参数错误
*/

#include <limits.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/* 定义word(unsigned int)的bit数 */
#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))

/*
computes the word that stores the bit corresponding to a given index
*/
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)

/* computes a mask to access the correct bit inside the word. */
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))

/*
set_array, get_array, and get_size have to check whether
they have got a valid array as their first argument.
To simplify their tasks, we define the following macro.
*/
#define checkarray(L) \
	(BitArray *)luaL_checkudata(L, 1, "LuaBook.array")

typedef struct BitArray
{
	int size;

	/*
	the array values with size 1 only as a placeholder,
	because C 89 does not allow an arraywith size 0
	*/
	unsigned int values[1]; /* variable part */
} BitArray;

static int new_array(lua_State *L)
{
	/* number of bits */
	int n = (int)luaL_checkinteger(L, 1);

	/* check n >= 1 is true */
	luaL_argcheck(L, n >= 1, 1, "invalid size");

	/* compute the array size in bytes */
	unsigned long long word_count = I_WORD(n - 1);
	size_t nbytes = sizeof(BitArray) + word_count * sizeof(unsigned int);

	/* create a userdata with the appropriate size, and push it to the stack */
	BitArray *a = (BitArray *)lua_newuserdata(L, nbytes);

	/* initialize its fields */
	a->size = n;
	for (int i = 0; i <= word_count; i++)
		a->values[i] = 0;

	/*
	retrieve metatable named "LuaBook.array" from the resistry and push onto stack
	*/
	luaL_getmetatable(L, "LuaBook.array");

	/*
	set metatable of the userdata
	*/
	lua_setmetatable(L, -2);

	return 1;
}

static unsigned int *get_params(lua_State *L, unsigned int *mask)
{
	/* check first argument is an valid array */
	BitArray *a = checkarray(L);

	/* second argument: the index, It assumes that indices start at one, as usual in Lua */
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");

	*mask = I_BIT(index); /* mask to access correct bit */

	return &a->values[I_WORD(index)]; /* word address */
}

static int set_array(lua_State *L)
{
	unsigned int mask;
	unsigned int *entry = get_params(L, &mask);

	/*
	third argument: the boolean value
	Because Lua accepts any value for a Boolean, we use luaL_checkany for the third parameter
	*/
	luaL_checkany(L, 3);

	/* set boolean value */
	if (lua_toboolean(L, 3))
		*entry |= mask;
	else
		*entry &= ~mask;

	return 0;
}

static int get_array(lua_State *L)
{
	unsigned int mask;
	unsigned int *entry = get_params(L, &mask);

	/* push value to stack */
	lua_pushboolean(L, *entry & mask);

	return 1;
}

static int get_size(lua_State *L)
{
	/* check first argument is an valid array */
	BitArray *a = checkarray(L);

	/* push size to stack */
	lua_pushinteger(L, a->size);

	return 1;
}

static const struct luaL_Reg array_lib[] = {
	{"new", new_array},
	{"set", set_array},
	{"get", get_array},
	{"size", get_size},
	{NULL, NULL},
};

int luaopen_arraymeta(lua_State *L)
{
	/* create a metatable and put in registry */
	luaL_newmetatable(L, "LuaBook.array");
	luaL_newlib(L, array_lib);
	return 1;
}
