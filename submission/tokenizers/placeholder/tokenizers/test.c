#include <stdio.h>

__attribute__((always_inline))
void do_stuff()
{
	puts("abcd");
}

void func()
{
	do_stuff();
}

int main()
{
	func();
}
