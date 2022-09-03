#pragma once

#include "lua.h"

void dump_stack(lua_State *L);

void error(lua_State *L, const char *fmt, ...);

void test_operate();

void simple_interpreter();

int secure_foo(lua_State *L);

void test_secure_foo();