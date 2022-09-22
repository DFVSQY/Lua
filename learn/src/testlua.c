
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
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

int get_global_int(lua_State *L, const char *var)
{
	int isnum, result;

	/*
	将全局变量对应的值放入stack，并返回该值对应的类型
	*/
	lua_getglobal(L, var);

	/*
	尝试将stack顶部的元素转换成lua_Integer，失败则返回0，
	参数3：用于标识是否成功转换
	*/
	result = (int)lua_tointegerx(L, -1, &isnum);

	if (!isnum)
		error(L, "'%s' should be a number\n", var);

	/*
	弹出一个stack元素，即移除stack顶部元素
	*/
	lua_pop(L, 1);
	return result;
}

void load_width_and_height(lua_State *L, const char *fname, int *w, int *h)
{
	/*
	luaL_loadfile：
		载入文件代码并编译成chunk块，如果没有异常，则将chunk块做为一个Lua Function压入stack，
		如果有异常，则将error message压入stack
		成功载入并编译返回0，否则返回其他错误码

	lua_pcall：
		从stack中弹出被压入的函数并在保护模式下执行代码，无错则返回0，有错则将错误信息放入stack中
	*/
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config, file:%s", lua_tostring(L, -1));

	*w = get_global_int(L, "width");
	*h = get_global_int(L, "height");
}

void test_load_width_and_height()
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

	int w, h;
	load_width_and_height(L, "learn\\lua\\win_config.lua", &w, &h);
	printf("width:%d, height:%d\n", w, h);

	lua_close(L);
}

lua_State *new_lua_state_with_win_cfg()
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
	luaL_loadfile：
		载入文件代码并编译成chunk块，如果没有异常，则将chunk块做为一个Lua Function压入stack，
		如果有异常，则将error message压入stack
		成功载入并编译返回0，否则返回其他错误码

	lua_pcall：
		从stack中弹出被压入的函数并在保护模式下执行代码，无错则返回0，有错则将错误信息放入stack中
	*/
	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config, file:%s", lua_tostring(L, -1));

	return L;
};

float get_color_filed(lua_State *L, const char *var)
{
	int result, isnum;
	lua_pushstring(L, var);

	/*
	table原本在stack顶部，压入string后变成了-2位置，
	*/
	lua_gettable(L, -2);

	result = (int)(lua_tonumberx(L, -1, &isnum) * MAX_COLOR);
	if (!isnum)
		error(L, "invalid component '%s' in color", var);
	lua_pop(L, 1);
	return result;
}

/* get_color_filed功能实现的简化版本 */
float get_color_filed_simple(lua_State *L, const char *var)
{
	/*
	lua_getfield：
	实现了lua_pushstring(L, var); lua_gettable(L, -2)组合的相同的功能，
	但具体实现上不会再把var压入stack中，table依旧位于stack顶部

	Pushes onto the stack the value t[k], where t is the value at the given index. As in Lua, this function may trigger a metamethod for the "index" event.
	Returns the type of the pushed value.
	*/
	if (lua_getfield(L, -1, var) != LUA_TNUMBER)
		error(L, "invalid component '%s' in color", var);

	/*
	Converts the Lua value at the given index to the C type lua_Number.
	The Lua value must be a number or a string convertible to a number;
	otherwise, lua_tonumberx returns 0.
	Equivalent to lua_tonumberx with isnum equal to NULL.
	*/
	int result = (int)(lua_tonumber(L, -1) * MAX_COLOR);

	lua_pop(L, 1);

	return result;
}

void get_rgb_color(lua_State *L, float *r, float *g, float *b)
{
	lua_getglobal(L, "background");
	if (!lua_istable(L, -1))
		error(L, "'background' is not a table");

	*r = get_color_filed(L, "r");
	*g = get_color_filed(L, "g");
	*b = get_color_filed(L, "b");
}

void get_rgb_color_simple(lua_State *L, float *r, float *g, float *b)
{
	lua_getglobal(L, "background");
	if (!lua_istable(L, -1))
		error(L, "'background' is not a table");

	*r = get_color_filed_simple(L, "r");
	*g = get_color_filed_simple(L, "g");
	*b = get_color_filed_simple(L, "b");
}

void test_get_rgb_color()
{
	lua_State *L = new_lua_state_with_win_cfg();

	float r, g, b;
	get_rgb_color(L, &r, &g, &b);
	printf("r:%.0f g:%.0f, b:%.0f\n", r, g, b);

	lua_close(L);
}

void test_get_rgb_color_simple()
{
	lua_State *L = new_lua_state_with_win_cfg();

	float r, g, b;
	get_rgb_color(L, &r, &g, &b);
	printf("r:%.0f g:%.0f, b:%.0f\n", r, g, b);

	lua_close(L);
}

void set_color_field(lua_State *L, const char *index, int value)
{
	lua_pushstring(L, index);					  /* key */
	lua_pushnumber(L, (double)value / MAX_COLOR); /* value */

	/*
	Does the equivalent to t[k] = v, where t is the value at the given index, v is the value at the top of the stack, and k is the value just below the top.
	This function pops both the key and the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event
	*/
	lua_settable(L, -3);
}

void set_color_field_simple(lua_State *L, const char *index, int value)
{
	lua_pushnumber(L, (double)value / MAX_COLOR); /* value */

	/*
	用于设置table元素的值，相当于免去了key入栈的过程，
	只有当key为字符串类型时才可以使用该简化版本，

	Does the equivalent to t[k] = v, where t is the value at the given index and v is the value at the top of the stack.
	This function pops the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event.
	*/
	lua_setfield(L, -2, index);
}

void set_global_color(lua_State *L, ColorTable *ct)
{
	int i = 0;
	while (1)
	{
		ColorTable *c = &ct[i++];
		if (c->name == NULL)
			break;

		/* Creates a new empty table and pushes it onto the stack */
		lua_newtable(L);

		set_color_field(L, "red", c->red);
		set_color_field(L, "green", c->green);
		set_color_field(L, "blue", c->blue);

		/* Pops a value from the stack and sets it as the new value of global name */
		lua_setglobal(L, c->name);
	}
}

void set_global_color_simple(lua_State *L, ColorTable *ct)
{
	int i = 0;
	while (1)
	{
		ColorTable *c = &ct[i++];
		if (c->name == NULL)
			break;

		/* Creates a new empty table and pushes it onto the stack */
		lua_newtable(L);

		set_color_field_simple(L, "red", c->red);
		set_color_field_simple(L, "green", c->green);
		set_color_field_simple(L, "blue", c->blue);

		/* Pops a value from the stack and sets it as the new value of global name */
		lua_setglobal(L, c->name);
	}
}

void test_set_global_color()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	ColorTable color_tables[] = {
		{"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
		{"RED", MAX_COLOR, 0, 0},
		{"GREEN", 0, MAX_COLOR, 0},
		{"BLUE", 0, 0, MAX_COLOR},
		{NULL, 0, 0, 0}};
	set_global_color(L, color_tables);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config, file:%s", lua_tostring(L, -1));

	lua_close(L);
}

void test_set_global_color_simple()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	ColorTable color_tables[] = {
		{"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
		{"RED", MAX_COLOR, 0, 0},
		{"GREEN", 0, MAX_COLOR, 0},
		{"BLUE", 0, 0, MAX_COLOR},
		{NULL, 0, 0, 0}};
	set_global_color_simple(L, color_tables);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config, file:%s", lua_tostring(L, -1));

	lua_close(L);
}

void test_cal_rect_area()
{
	lua_State *L = new_lua_state_with_win_cfg();

	double w = 10, h = 20;

	// 将需要的调用的lua函数压入stack
	lua_getglobal(L, "CalRectArea");

	// 将函数需要的参数width，height压入stack
	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	/*
	执行函数调用，并将函数及其参数从stack中弹出，将计算结果压入stack，
	如果函数运行报错，lua_pcall依旧会将函数及其参数弹出stack，并将一个‘error message’压入stack，
	在将message压入stack时，lua_pcall会调用第4个参数（0表示没有，其他表示在stack的位置）指定的‘message handler’(如果有的话)进行处理。
	*/
	if ((lua_pcall(L, 2, 1, 0) != LUA_OK))
		error(L, "error running function 'CalRectArea':%s", lua_tostring(L, -1));

	int isnum;
	double area = lua_tonumberx(L, -1, &isnum); // 读取计算结果
	if (!isnum)
		error(L, "function 'CalRectArea' should return a number");
	lua_pop(L, 1); // 弹出计算结果

	printf("area result:%f\n", area);

	lua_close(L);

	return;
}

/*
调用lua函数的通用版本
func: 待调用函数的名称
sig：指明每个参数和返回值的类型
...：传入的参数和接收返回值的指针变量
*/
void call_func_common(lua_State *L, const char *func, const char *sig, ...)
{
	int narg, nres; // 参数个数，返回值个数

	va_list vl;
	va_start(vl, sig);

	/* 待调用函数入栈 */
	lua_getglobal(L, func);

	/* 参数入栈 */
	for (narg = 0; *sig; narg++)
	{
		/*
		Grows the stack size to top + sz elements, raising an error if the stack cannot grow to that size.
		msg is an additional text to go into the error message (or NULL for no additional text).
		*/
		luaL_checkstack(L, 1, "too many args");

		switch (*sig++)
		{
		case 'd':
			lua_pushnumber(L, va_arg(vl, double));
			break;

		case 'i':
			lua_pushnumber(L, va_arg(vl, int));
			break;

		case 's':
			lua_pushstring(L, va_arg(vl, char *));
			break;

		case '>':
			goto endargs;

		default:
			error(L, "invalid option (%c)", *(sig - 1));
		}
	}
endargs:

	/* 期望的返回结果个数*/
	nres = strlen(sig);

	if (lua_pcall(L, narg, nres, 0) != 0)
		error(L, "error caling '%s':%s", func, lua_tostring(L, -1));

	/* 接收返回值 */
	nres = -nres; /* stack index of the first result */

	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
		{
			int isnum;
			double n = lua_tonumberx(L, nres, &isnum);
			if (!isnum)
				error(L, "wrong result type");
			*va_arg(vl, double *) = n;
			break;
		}
		case 'i':
		{
			int isnum;
			int n = lua_tointegerx(L, nres, &isnum);
			if (!isnum)
				error(L, "wrong result type");
			*va_arg(vl, int *) = n;
			break;
		}
		case 's':
		{
			const char *s = lua_tostring(L, nres);
			if (s == NULL)
				error(L, "wrong result type");
			*va_arg(vl, const char **) = s;
			break;
		}
		default:
			error(L, "invlid option (%c)", *(sig - 1));
		}
		nres++;
	}

	va_end(vl);
}

void test_call_func_common()
{
	lua_State *L = new_lua_state_with_win_cfg();

	double w = 10, h = 20;
	double area;

	call_func_common(L, "CalRectArea", "dd>d", w, h, &area);

	lua_pop(L, 1); // 弹出计算结果

	printf("area result:%f\n", area);

	lua_close(L);

	return;
}

static int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L, 1); /* 从stack中获取参数 */
	lua_pushnumber(L, sin(d));		   /* 将计算结果压入stack */
	return 1;						   /* 返回结果个数 */
}

static void reg_l_sin(lua_State *L)
{
	/*
	Pushes a C function onto the stack. This function receives a pointer to a C function and pushes onto the stack a Lua value of type function that,
	when called, invokes the corresponding C function.
	Any function to be callable by Lua must follow the correct protocol to receive its parameters and return its results
	*/
	lua_pushcfunction(L, l_sin);

	/*
	Pops a value from the stack and sets it as the new value of global name
	*/
	lua_setglobal(L, "c_sin");
}

static int get_dir_files(lua_State *L)
{
	DIR *dir;
	const char *path = luaL_checkstring(L, 1); /* 读取目录 */
	dir = opendir(path);					   /* 打开目录 */
	if (dir == NULL)						   /* 打开目录错误 */
	{
		lua_pushnil(L);						/* return nil, ... */
		lua_pushstring(L, strerror(errno)); /* push error message */
		return 2;							/* number of results */
	}

	struct dirent *entry;
	lua_newtable(L);
	int i = 1;
	while ((entry = readdir(dir)) != NULL) /* for each entry */
	{
		lua_pushinteger(L, i++);		  /* push key */
		lua_pushstring(L, entry->d_name); /* push value */
		lua_settable(L, -3);			  /* table[i] = entry name */
	}

	closedir(dir);
	return 1; /* number of results */
}

static void reg_get_dir_files(lua_State *L)
{
	lua_pushcfunction(L, get_dir_files);
	lua_setglobal(L, "c_get_dir_files");
}

static void reg_get_dir_files_2(lua_State *L)
{
	/*
	以下两句的缩写：
	lua_pushcfunction(L, get_dir_files);
	lua_setglobal(L, "c_get_dir_files");
	*/
	lua_register(L, "c_get_dir_files_2", get_dir_files);
}

void test_lua_call_c_func()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	reg_l_sin(L);
	reg_get_dir_files(L);
	reg_get_dir_files_2(L);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}

void test_lua_call_c_module_func()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushboolean(L, 1);
	lua_setglobal(L, "use_math_lib");

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}

static void operate_array(lua_State *L)
{
	/* 确保第一个参数为table类型 */
	luaL_checktype(L, 1, LUA_TTABLE);

	/* 确保第二个参数为function类型 */
	luaL_checktype(L, 2, LUA_TFUNCTION);

	/* 返回table的数组长度 */
	int n = luaL_len(L, 1);

	for (int i = 1; i <= n; i++)
	{
		/* 将函数f压入stack */
		lua_pushvalue(L, 2);

		/* 将t[i]压入stack */
		lua_geti(L, 1, i);

		/* 调用函数: f(t[i]) */
		lua_call(L, 1, 1);

		/* t[i] = result */
		lua_seti(L, 1, i);
	}

	for (int i = 1; i <= n; i++)
	{
		lua_geti(L, 1, i);
		double num = luaL_checknumber(L, -1);
		lua_pop(L, 1);
		printf("%.0f\n", num);
	}
}

void test_operate_array()
{
	lua_State *L = new_lua_state_with_win_cfg();

	lua_getglobal(L, "array_t");
	lua_getglobal(L, "each_array_e");
	operate_array(L);

	lua_close(L);
};

static int split_string(lua_State *L)
{
	const char *s = luaL_checkstring(L, 1);	  /* 待分割字符串 */
	const char *sep = luaL_checkstring(L, 2); /* 分割字符 */

	lua_newtable(L); /* 新建一个table并压入stack */

	int i = 1;
	const char *e;
	while ((e = strchr(s, *sep)) != NULL) /* e为遇见的第一个分隔符的指针 */
	{
		lua_pushlstring(L, s, e - s); /* 分割的子字符串压入stack */
		lua_rawseti(L, -2, i++);	  /* t[i] = substr */
		s = e + 1;
	}

	lua_pushstring(L, s);  /* 最后一个substring压入stack */
	lua_rawseti(L, -2, i); /* t[i] = substr */

	return 1;
}

void reg_split_string(lua_State *L)
{
	lua_pushcfunction(L, split_string);
	lua_setglobal(L, "c_split_string");
}

void test_operate_string()
{
	/*
	当C函数接收到来自Lua的一个字符串参数时，只需要遵守两条规则：
	1. 不要将string从stack中弹出
	2. 永远不要修改字符串内容
	*/

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	reg_split_string(L);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}

static int tconcat(lua_State *L)
{
	/* 初始化一个用于存储字符串内容的Buffer */
	luaL_Buffer b;

	/* 检测第一个参数是否为table类 */
	luaL_checktype(L, 1, LUA_TTABLE);

	/* 获取table的数组长度 */
	int n = luaL_len(L, 1);

	/* 初始化buffer，但不分配任何空间，将指定的lua_State绑定到buffer上 */
	luaL_buffinit(L, &b);

	for (int i = 1; i <= n; i++)
	{
		/* 获取t[i]的值并压入stack */
		lua_geti(L, 1, i);

		/* 将stack顶部的值加入buffer，并弹出stack顶部值 */
		luaL_addvalue(&b);
	}

	/* 完成buffer的使用，并将组合成的lua字符串压入stack */
	luaL_pushresult(&b);

	return 1;
}

void reg_tconcat(lua_State *L)
{
	lua_pushcfunction(L, tconcat);
	lua_setglobal(L, "c_tconcat");
}

void test_tconcat()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	reg_tconcat(L);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}

static void use_registry_store_lua_value_2_c(lua_State *L)
{
	/*
	使用registry的一种方式是使用luaL_ref获取唯一整数key进行table存取,
	示例如下:
	*/

	/* 弹出stack顶值，放入registry table中*/
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);

	/* 取出registry table中的指定的ref引用的值放入stack顶部 */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

	/* 释放registry中的ref占用 */
	luaL_unref(L, LUA_REGISTRYINDEX, ref);

	/*
	使用registry的一种方式是使用static变量的地址（c链接器会确保该地址唯一）作为key进行table存取,
	示例如下:
	*/

	static char key = 'k';

	/* 存字符串到registry */
	lua_pushlightuserdata(L, (void *)&key); /* push address key */
	lua_pushstring(L, "hello world");		/* push value */
	lua_settable(L, LUA_REGISTRYINDEX);		/* registry[key] = "hello world" */

	/* 取字符串从registry */
	lua_pushlightuserdata(L, (void *)&key); /* push address key */
	lua_gettable(L, LUA_REGISTRYINDEX);		/* get value  */
	const char *str = lua_tostring(L, -1);	/* convert to string */
	printf(str);
}

/* use_registry_store_lua_value_2_c 的简化版 */
static void use_registry_store_lua_value_2_c_simple(lua_State *L)
{
	/*
	使用registry的一种方式是使用luaL_ref获取唯一整数key进行table存取,
	示例如下:
	*/

	/* 弹出stack顶值，放入registry table中*/
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);

	/* 取出registry table中的指定的ref引用的值放入stack顶部 */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

	/* 释放registry中的ref占用 */
	luaL_unref(L, LUA_REGISTRYINDEX, ref);

	/*
	使用registry的一种方式是使用static变量的地址（c链接器会确保该地址唯一）作为key进行table存取,
	示例如下:
	*/

	static char key = 'k';

	/* 存字符串到registry */
	lua_pushstring(L, "hello world");				 /* push value */
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void *)&key); /* registry[key] = "hello world" */

	/* 取字符串从registry */
	lua_rawgetp(L, LUA_REGISTRYINDEX, (void *)&key);
	const char *str = lua_tostring(L, -1); /* convert to string */
	printf(str);
}

void test_use_registry()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	use_registry_store_lua_value_2_c(L);
	use_registry_store_lua_value_2_c_simple(L);
	lua_close(L);
}

static int counter(lua_State *L)
{
	/*
	lua_upvalueindex, which produces the pseudo-index of an upvalue.
	In particular, the expression lua_upvalueindex(1) gives the pseudo-index of the first upvalue of the running function.
	Again, this pseudo-index is like any stack index, except that it does not live on the stack
	*/
	int idx = lua_upvalueindex(1);

	/* lua_tointeger retrieves the first upvalue */
	int val = lua_tointeger(L, idx);

	/* use the upvalue and put the result to stack */
	lua_pushinteger(L, ++val);

	/* adjust upvalue's value, here copy the result to the upvalue's value */
	lua_copy(L, -1, idx);
	return 1;
};

static int new_counter(lua_State *L)
{
	/* push upvalue(here is 0) to stack */
	lua_pushinteger(L, 0);

	/*
	create a closure
	second arg: the base function
	third arg: the number of upvalues

	Before creating a new closure, we must push on the stack the initial values for its upvalues.
	lua_pushcclosure leaves the new closure on the stack,
	so the closure is ready to be returned as the result of new_counter
	*/
	lua_pushcclosure(L, &counter, 1);
	return 1;
}

void test_c_closure()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "c_counter", new_counter);

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
};

void test_c_closure_tuple()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushboolean(L, 1);
	lua_setglobal(L, "use_tuple_lib");

	const char *fname = "learn\\lua\\win_config.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}

void test_c_array()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	const char *fname = "learn\\lua\\userdata.lua";
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config file, error msg:%s", lua_tostring(L, -1));

	lua_close(L);
}