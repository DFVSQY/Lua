#pragma once

#include "lua.h"

#define MAX_COLOR 255

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

float get_color_filed(lua_State *L, const char *var);

void test_get_rgb_color();