cmake_minimum_required(VERSION 3.24)

project(LearnLua)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(. ./learn/headers)
add_compile_options(-Wall -g)

# ------------------------------ Lua Dll ---------------------------------
set(LUA_DLL
	lapi.c
	lauxlib.c
	lbaselib.c
	lbitlib.c
	lcode.c
	lcorolib.c
	lctype.c
	ldblib.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	linit.c
	liolib.c
	llex.c
	lmathlib.c
	lmem.c
	loadlib.c
	lobject.c
	lopcodes.c
	loslib.c
	lparser.c
	lstate.c
	lstring.c
	lstrlib.c
	ltable.c
	ltablib.c
	ltests.c
	ltm.c
	lundump.c
	lutf8lib.c
	lvm.c
	lzio.c
)
add_library(lua SHARED ${LUA_DLL})
set_target_properties(lua PROPERTIES PREFIX "")

# ------------------------------ DLL and Main EXE ---------------------------------
# mymath dll
set(MYMATH_DLL
	learn/src/mymath.c
)
add_library(mymathlib SHARED ${MYMATH_DLL})
target_link_libraries(mymathlib PUBLIC lua)
set_target_properties(mymathlib PROPERTIES PREFIX "")

# mytuple dll
set(MYMATH_DLL
	learn/src/mytuple.c
)
add_library(mytuplelib SHARED ${MYMATH_DLL})
target_link_libraries(mytuplelib PUBLIC lua)
set_target_properties(mytuplelib PROPERTIES PREFIX "")

# array dll
set(MYARRAY_DLL
	learn/src/array.c
)
add_library(array SHARED ${MYARRAY_DLL})
target_link_libraries(array PUBLIC lua)
set_target_properties(array PROPERTIES PREFIX "")

# arraymeta dll
set(MYARRAY_META_DLL
	learn/src/arraymeta.c
)
add_library(arraymeta SHARED ${MYARRAY_META_DLL})
target_link_libraries(arraymeta PUBLIC lua)
set_target_properties(arraymeta PROPERTIES PREFIX "")

# arrayobj dll
set(MYARRAY_OBJ_DLL
	learn/src/arrayobj.c
)
add_library(arrayobj SHARED ${MYARRAY_OBJ_DLL})
target_link_libraries(arrayobj PUBLIC lua)
set_target_properties(arrayobj PROPERTIES PREFIX "")

# dir dll
set(MY_DIR_DLL
	learn/src/dir.c
)
add_library(dir SHARED ${MY_DIR_DLL})
target_link_libraries(dir PUBLIC lua)
set_target_properties(dir PROPERTIES PREFIX "")

# lxp dll
set(MY_DIR_DLL
	learn/src/lxp.c
	learn/src/expat/xmlparse.c
	learn/src/expat/xmlrole.c
	learn/src/expat/xmltok_impl.c
	learn/src/expat/xmltok_ns.c
	learn/src/expat/xmltok.c
)
add_library(lxp SHARED ${MY_DIR_DLL})
target_include_directories(lxp PRIVATE learn/headers/expat)
target_link_libraries(lxp PUBLIC lua)
set_target_properties(lxp PROPERTIES PREFIX "")

# main exe
set(MAIN_SRC
	learn/src/main.c
	learn/src/testlua.c
)
add_executable(main ${MAIN_SRC})
target_link_libraries(main PRIVATE lua)

# ------------------------------ Main Src ---------------------------------
set(MAIN_SRC
	learn/src/main_src.c
	learn/src/testlua.c
	lapi.c
	lauxlib.c
	lbaselib.c
	lbitlib.c
	lcode.c
	lcorolib.c
	lctype.c
	ldblib.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	linit.c
	liolib.c
	llex.c
	lmathlib.c
	lmem.c
	loadlib.c
	lobject.c
	lopcodes.c
	loslib.c
	lparser.c
	lstate.c
	lstring.c
	lstrlib.c
	ltable.c
	ltablib.c
	ltests.c
	ltm.c
	lundump.c
	lutf8lib.c
	lvm.c
	lzio.c
)
add_executable(main_src ${MAIN_SRC})