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

void set_global_color(lua_State *L, ColorTable *ct);

void test_set_global_color();