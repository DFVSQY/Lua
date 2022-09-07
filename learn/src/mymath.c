/*
该文件用于创建mymath.dll动态库
*/

#include "lua.h"
#include "lauxlib.h"
#include "mymath.h"

static int add(lua_State *L)
{
	double a = luaL_checknumber(L, 1);
	double b = luaL_checknumber(L, 2);
	double c = a + b;
	lua_pushnumber(L, c);
	return 1;
}

static int sub(lua_State *L)
{
	double a = luaL_checknumber(L, 1);
	double b = luaL_checknumber(L, 2);
	double c = a - b;
	lua_pushnumber(L, c);
	return 1;
}

static const struct luaL_Reg my_math[] = {
	{"add", add},
	{"sub", sub},
	{NULL, NULL}};

/*
在lua脚本中调用: 'require "mymathlib"'

This call links the dynamic library 'mymathlib' with Lua, finds the function luaopen_mymathlib, 
registers it as a C function, and calls it, opening the module. 
(This behavior explains why luaopen_mymathlib must have the same prototype as any other C function.)

The dynamic linker must know the name of the function 'luaopen_mymathlib' in order to find it. 
It will always look for 'luaopen_' concatenated with the name of the module. 
Therefore, if our module is called 'mymathlib', that function should be called 'luaopen_mylib'. 
*/
int luaopen_mymathlib(lua_State *L)
{
	/*
	The call to luaL_newlib creates a new table and fills it with the pairs name–function specified by the array my_math.
	When it returns, luaL_newlib leaves on the stack the new table wherein it opened the library.
	The function luaopen_mylib then returns 1 to return this table to Lua.
	*/
	luaL_newlib(L, my_math);
	return 1;
}