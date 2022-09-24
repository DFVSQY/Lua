/*
目录功能相关的处理
*/

#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"

static int dir_iter(lua_State *L)
{
	/* 获取关联的第一个upvalue的伪索引 */
	int idx = lua_upvalueindex(1);

	/* 取出userdata，即目录结构的指针 */
	DIR *d = *(DIR **)lua_touserdata(L, idx);

	/* 读取下一个entry */
	struct dirent *entry = readdir(d);
	if (entry != NULL)
	{
		lua_pushstring(L, entry->d_name);
		return 1;
	}
	else
	{
		return 0;
	}
}

static int l_dir(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	DIR **d = (DIR **)lua_newuserdata(L, sizeof(DIR *));
	*d = NULL;

	luaL_getmetatable(L, "LuaBook.dir");

	/* 为userdata设置metatable */
	lua_setmetatable(L, -2);

	*d = opendir(path);
	if (*d == NULL)
		luaL_error(L, "cannot open %s: %s", path, strerror(errno));

	/* 创建一个闭包函数，包含一个upvalue，即创建的userdata */
	lua_pushcclosure(L, dir_iter, 1);
	return 1;
}

static int dir_gc(lua_State *L)
{
	printf("c side dir_gc call\n");
	/* 获取待释放处理的userdata */
	DIR *d = *(DIR **)lua_touserdata(L, 1);
	if (d)
		closedir(d);
	return 0;
}

static const struct luaL_Reg dir_lib[] = {
	{"open", l_dir},
	{NULL, NULL},
};

int luaopen_dir(lua_State *L)
{
	luaL_newmetatable(L, "LuaBook.dir");

	/* mt.__gc = dir_gc */
	lua_pushcfunction(L, dir_gc);
	lua_setfield(L, -2, "__gc");

	luaL_newlib(L, dir_lib);
	return 1;
}