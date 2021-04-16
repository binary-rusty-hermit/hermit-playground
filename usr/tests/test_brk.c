#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

volatile ssize_t a;
volatile ssize_t ret_g;

int main(int argc, char** argv)
{
	// Get program brk
	a = 0;

	asm volatile  ( "mov    $12, %%rax\n\t"
                        "mov    a, %%rdi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi"
                        );

	printf("%zd was returned from brk\n\n", ret_g);

	// add 10
	a = ret_g + 10;

	asm volatile  ( "mov    $12, %%rax\n\t"
                        "mov    a, %%rdi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi"
                        );

        printf("%zd was returned from brk\n\n", ret_g);

	// Test ENOMEM
	a = ret_g + 10000000000;

	asm volatile  ( "mov    $12, %%rax\n\t"
                        "mov    a, %%rdi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi"
                        );

        printf("%zd was returned from brk\n\n", ret_g);

	return 0;
}
