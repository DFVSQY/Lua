#pragma once

#include "lua.h"

#define MAX_COLOR 255

typedef struct
{
	char *name;
	unsigned char red, green, blue;
} ColorTable;

void dump_stack(lua_State *L);

void error(lua_State *L, const char *fmt, ...);

void test_operate();

void simple_interpreter();

int secure_foo(lua_State *L);

void test_secure_foo();

int get_global_int(lua_State *L, const char *var);

void load_width_and_height(lua_State *L, const char *fname, int *w, int *h);

void test_load_width_and_height();

lua_State *new_lua_state_with_win_cfg();

void get_rgb_color(lua_State *L, float *r, float *g, float *b);

void get_rgb_color_simple(lua_State *L, float *r, float *g, float *b);

float get_color_filed(lua_State *L, const char *var);

float get_color_filed_simple(lua_State *L, const char *var);

void test_get_rgb_color();

void test_get_rgb_color_simple();

void set_color_field(lua_State *L, const char *index, int value);

void set_color_field_simple(lua_State *L, const char *index, int value);

void set_global_color(lua_State *L, ColorTable *ct);

void set_global_color_simple(lua_State *L, ColorTable *ct);

void test_set_global_color();

void test_set_global_color_simple();

void test_cal_rect_area();

void call_func_common(lua_State *L, const char *func, const char *sig, ...);

void test_call_func_common();

void test_lua_call_c_func();

void test_lua_call_c_module_func();

void test_operate_array();

void test_operate_string();

void reg_split_string();

void reg_tconcat(lua_State *L);

void test_tconcat();

void test_use_registry();

void test_c_closure();

void test_c_closure_tuple();

void test_c_array();

void test_c_dir();

void test_c_xml();