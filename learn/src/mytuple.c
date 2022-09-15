#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "mytuple.h"

static int t_tuple(lua_State *L)
{
	/*
	由于设计的tuple可以接受无参或者整数参数

	该函数用于检测是否存在指定位置的stack值并可转换成整数，如果可以则返回该整数，否则返回默认值0

	second arg: 指定的stack位置
	third arg: default value
	*/
	lua_Integer op = luaL_optinteger(L, 1, 0);

	if (op == 0)
	{
		int i;
		for (i = 0; !lua_isnone(L, lua_upvalueindex(i)); i++) /* 迭代所有upvalue值 */
			lua_pushvalue(L, lua_upvalueindex(i));			  /* push upvalue to stack */
		return i - 1;
	}
	else
	{
		/*
		The maximum number of upvalues to a C function is 255,
		and the maximum index we can use with lua_upvalueindex is 256.
		So, we use luaL_argcheck to ensure these limits.

		The third argument to luaL_argcheck provides the argument number for the error message (1, in the example),
		and the fourth argument provides a complement to the message ("index out of range").
		*/
		luaL_argcheck(L, 0 < op && op <= 256, 1, "index out of range");

		/*
		When we index a non-existent upvalue, the result is a pseudo-value whose type is LUA_TNONE.
		(When we access a stack index above the current top, we also get a pseudo-value with this type LUA_TNONE.)
		Our function t_tuple uses lua_isnone to test whether it has a given upvalue

		However, we should never use lua_upvalueindex with a negative index or with an index greater than 256
		(which is one plus the maximum number of upvalues for a C function),
		so we must check for this condition when the user provides the index
		*/
		if (lua_isnone(L, lua_upvalueindex(op)))
			return 0;

		/*
		push upvalue to stack
		*/
		lua_pushvalue(L, lua_upvalueindex(op));
		return 1;
	}
}

static int t_new(lua_State *L)
{
	int top = lua_gettop(L);

	/*
	first checks that the number of fields respects the limit for upvalues in a closure
	*/
	luaL_argcheck(L, top < 256, top, "too many fileds");

	/*
	create a closure
	second arg: the base function
	third arg: the number of upvalues

	Before creating a new closure, we must push on the stack the initial values for its upvalues.
	lua_pushcclosure leaves the new closure on the stack,
	so the closure is ready to be returned as the result of new_counter
	*/
	lua_pushcclosure(L, t_tuple, top);
	return 1;
}

static const struct luaL_Reg tuple_lib[] = {
	{"new", t_new},
	{NULL, NULL},
};

int luaopen_mytuplelib(lua_State *L)
{
	luaL_newlib(L, tuple_lib);

	/*
	共享upvalue的版本:
	// create library table ('lib' is its list of functions)
	luaL_newlibtable(L, tuple_lib);

	// create shared upvalue
	lua_newtable(L);

	// add functions in list 'lib' to the new library, sharing previous table as upvalue
	luaL_setfuncs(L, tuple_lib, 1);
	*/

	return 1;
}