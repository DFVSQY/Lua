#include <stdio.h>
#include "testlua.h"

void test_va(const char *prefix, ...)
{
	va_list vl;
	va_start(vl, prefix);
	int a = va_arg(vl, int);
	char *str = va_arg(vl, char *);
	double d = va_arg(vl, double);
	va_end(vl);
	printf("%s:%d:%s:%f\n", prefix, a, str, d);
}

int main()
{
	int a = 10;
	double d = 3.1415;
	char *str = "vscode";
	test_va("test", a, str, d);
	printf("run finish!\n");
	return 0;
}