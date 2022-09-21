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

	return 1;
}

static int set_array(lua_State *L)
{
	/* first argument: the array */
	BitArray *a = (BitArray *)lua_touserdata(L, 1);
	luaL_argcheck(L, a != NULL, 1, "'array' expected");

	/* second argument: the index, It assumes that indices start at one, as usual in Lua */
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");

	/*
	third argument: the boolean value
	Because Lua accepts any value for a Boolean, we use luaL_checkany for the third parameter
	*/
	luaL_checkany(L, 3);

	/* set boolean value */
	if (lua_toboolean(L, 3))
		a->values[I_WORD(index)] |= I_BIT(index);
	else
		a->values[I_WORD(index)] &= ~I_BIT(index);

	return 0;
}

static int get_array(lua_State *L)
{
	/* first argument: the array */
	BitArray *a = (BitArray *)lua_touserdata(L, 1);
	luaL_argcheck(L, a != NULL, 1, "'array' expected");

	/* second argument: the index, It assumes that indices start at one, as usual in Lua */
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");

	/* push value to stack */
	lua_pushboolean(L, a->values[I_WORD(index)] & I_BIT(index));

	return 1;
}

static int get_size(lua_State *L)
{
	/* first argument: the array */
	BitArray *a = (BitArray *)lua_touserdata(L, 1);
	luaL_argcheck(L, a != NULL, 1, "'array' expected");

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

int luaopen_array(lua_State *L)
{
	luaL_newlib(L, array_lib);
	return 1;
}
