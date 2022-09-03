
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

static int foo(lua_State *L)
{
	float a = 5, b = 0;
	float c = a / b;
	printf("foo call, c:%f\n", c);
	return 0;
}

int secure_foo(lua_State *L)
{
	/*
	将一个c函数压入stack中
	*/
	lua_pushcfunction(L, foo);

	/*
	在保护模式下调用函数，如果调用的函数没有任何报错，则该函数运行行为类似lua_call,
	如果报错，则lua_pcall捕获错误，并将一个‘error object’压入stack，并返回一个错误码，
	类似于lua_call，lua_pcall函数总是会从stack中移除调用的函数及其参数
	参数2：用到的参数个数
	参数3：返回值个数
	参数4：如果为0，则返回原始的‘error object’压入stack，否则该函数表示stack的一个索引，该索引
			对应的函数为错误处理函数，该函数的返回值即为压入stack的‘error object’
	*/
	int result = lua_pcall(L, 0, 0, 0);

	return (result == 0);
}

void test_secure_foo()
{
	/*
	创建一个lua状态机，此时lua环境不包含任何预定义函数，甚至print也不包含，
	为了使lua足够小，所有的标准库都是以独立的package存在，如果不需要就可以不使用它们
	*/
	lua_State *L = luaL_newstate();

	/*
	该函数用于打开所有的lua标准库
	*/
	luaL_openlibs(L);

	/* 
	对于绝大部分主要的Lua APIs，任何一次调用都有可能引起异常错误，为了正确地处理这些错误，
	必须在lua环境（存在一个可以catch error的context）中调用这些函数，这样自己写的c代码就
	可以在保护模式下执行了
	*/
	printf("secure_foo call, result:%d\n", secure_foo(L));

	lua_close(L);
}