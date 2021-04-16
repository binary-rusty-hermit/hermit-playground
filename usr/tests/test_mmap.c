#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

volatile void* addr;
volatile size_t len;
volatile size_t prot;
volatile size_t flags;
volatile size_t fd_1;
volatile size_t offset; 
volatile ssize_t ret_g;

int main(int argc, char** argv)
{
	addr = NULL;
	len = 4096;
	prot = 0x1 | 0x2; // read and write
	flags = 0x2 | 0x20; // private and map anonymous
	fd_1 = -1;
	offset = 0;

	// Mmap
	asm volatile  ( "mov    $9, %%rax\n\t"
                        "mov    addr, %%rdi\n\t"
			"mov 	len, %%rsi\n\t"
			"mov    prot,  %%rdx\n\t"
			"mov    flags, %%r10\n\t"
			"mov    fd_1, %%r8\n\t"
			"mov    offset, %%r9\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"
                        );
	printf("%zd was returned from mmap\n\n", ret_g);
	
	int* int_ptr = ret_g;
	printf("%i: ", int_ptr);
	*int_ptr = 1;
	printf("%i\n", *int_ptr);
	

	addr = ret_g;

	// Munmap
	asm volatile  ( "mov    $11, %%rax\n\t"
                        "mov    addr, %%rdi\n\t"
			"mov    len, %%rsi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi"
                        );

	printf("%zd was returned from munmap\n\n", ret_g);

/*	// Munmap should panic
        asm volatile  ( "mov    $11, %%rax\n\t"
                        "mov    addr, %%rdi\n\t"
                        "mov    len, %%rsi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi"
                        );*/

	// Mprotect
	asm volatile  ( "mov    $10, %%rax\n\t"
                        "mov    addr, %%rdi\n\t"
                        "mov    len, %%rsi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi"
                        );
	printf("%zd was returned from mprotect\n\n", ret_g);

	return 0;
}
