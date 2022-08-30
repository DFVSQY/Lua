#include <stdio.h>
#include <string.h>


/*
'lua.h'中声明了lua提供的基础函数,包括创建lua环境，调用lua函数，读写lua环境中的全局变量，注册可被lua调用的函数等等，
所有'lua.h'中的函数都以'lua_'前缀开头
*/
#include "lua.h"

/*
'lauxlib.h'提供了辅助库'lauxlib.c'中的函数声明，其所有函数都以'luaL_'前缀开头，主要使用'lua.h'中声明的函数提供了一层
更高层次的包装，一些通用工作会用到这些封装
*/
#include "lauxlib.h"

/*
定义一些用于打开lua库的函数
*/
#include "lualib.h"

int main()
{
	char buff[256];
	int error;

	/*
	创建一个lua状态机，此时lua环境不包含任何预定义函数，甚至print也不包含，
	为了使lua足够小，所有的标准库都是以独立的package存在，如果不需要就可以不使用它们
	*/
	lua_State *L = luaL_newstate();

	/*
	该函数用于打开所有的lua标准库
	*/
	luaL_openlibs(L);

	while (fgets(buff, sizeof(buff), stdin) != NULL)
	{
		/*
		luaL_loadstring:用于编译输入的lua代码，无错返回0，有错则将错误信息放入stack中，并将生成的函数放入stack中
		lua_pcall:从stack中弹出被压入的函数并在保护模式下执行代码，无错则返回0，有错则将错误信息放入stack中
		*/
		error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);

		if (error)
		{
			/*
			lua_tostring：获取压入stack的错误信息，只获取不弹出
			*/
			fprintf(stderr, "%s\n", lua_tostring(L, -1));

			/*
			从stack中弹出错误信息
			*/
			lua_pop(L, 1);
		}
	}

	lua_close(L);
	return 0;
}