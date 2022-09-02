
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "testlua.h"

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

void dump_stack(lua_State *L)
{
	int top = lua_gettop(L); /* 获取顶部元素索引，即总元素个数 */
	for (int i = 1; i <= top; i++)
	{
		int t = lua_type(L, i); /* 获取元素类型 */
		switch (t)
		{
		case LUA_TSTRING:
		{
			printf("'%s'", lua_tostring(L, i)); // 转换成字符串类型
			break;
		}

		case LUA_TBOOLEAN:
		{
			printf(lua_toboolean(L, i) ? "true" : "false"); // bool类型
			break;
		}

		case LUA_TNUMBER:
		{
			if (lua_isinteger(L, i))
			{
				printf("%lld", lua_tointeger(L, i));
			}
			else
			{
				printf("%g", lua_tonumber(L, i));
			}
			break;
		}

		default:
		{
			printf("%s", lua_typename(L, t)); // 其他类型名称
			break;
		}
		}
		printf("	");
	}
	printf("\n");
}

void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L); /*发生错误，直接关闭lua状态机*/
	exit(EXIT_FAILURE);
}

void test_operate()
{
	lua_State *L = luaL_newstate();

	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");

	/*
	输出：true,  10, nil, 'hello'
	*/
	dump_stack(L);

	/*
	拷贝-4位置处的元素拷贝一份放入栈顶
	*/
	lua_pushvalue(L, -4);

	/*
	输出：true, 10, nil, 'hello', true
	*/
	dump_stack(L);

	/*
	使用栈顶元素替换3位置处的元素并移除栈顶元素
	*/
	lua_replace(L, 3);

	/*
	输出：true, 10, true, 'hello'
	*/
	dump_stack(L);

	/*
	设置stack元素个数，此处扩展两个空位
	*/
	lua_settop(L, 6);

	/*
	输出：true, 10, true, 'hello', nil, nil
	*/
	dump_stack(L);

	/*
	循环移动后四位元素
	*/
	lua_rotate(L, 3, 1);

	/*
	输出：true, 10, nil, true, 'hello', nil
	*/
	dump_stack(L);

	/*
	移除第三高位的元素
	*/
	lua_remove(L, -3);

	/*
	输出：true, 10, nil, 'hello' nil
	*/
	dump_stack(L);

	/*
	设置stack元素个数，此处移除后四位元素
	*/
	lua_settop(L, -5);

	/*
	输出：true
	*/
	dump_stack(L);

	lua_close(L);
}

void simple_interpreter()
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
}
