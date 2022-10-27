#include "testlua.h"
#include <stdio.h>

int main()
{
    printf("-------------------------\n");
    test_lua_call_c_module_func();
    printf("-------------------------\n");
    test_c_closure_tuple();
    printf("-------------------------\n");
    test_c_array();
    printf("-------------------------\n");
    test_c_dir();
    printf("-------------------------\n");
    test_c_xml();
    printf("-------------------------\n");
    printf("run finish!\n");
    return 0;
}