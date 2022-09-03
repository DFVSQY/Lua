#include <stdio.h>
#include <setjmp.h>
#include "testlua.h"

jmp_buf buf;

void func()
{
	printf("action func in step 1\n");

	/*  
	longjmp: go back to place buf is pointing to and return second param(here is 1)
	*/
	longjmp(buf, 1);

	printf("action func in step 2\n");
}

/*
输出:
main action2
action func in step 1
main action1
main action4
*/
int main()
{
	/* 
	setjmp: uses buf to remember current position and returns 0
	*/
	if (setjmp(buf))
	{
		printf("main action1\n");
	}
	else
	{
		printf("main action2\n");
		func();
		printf("main action3\n");
	}

	printf("main action4\n");

	return 0;
}