#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


/* For the write_asm function, the C variables used inside the asm()
   construct need to have global scope. */
volatile size_t fd_g = 0;
volatile char *buf_g;
volatile size_t len_g;
volatile ssize_t ret_g;

static ssize_t write_asm(int fd, char *buf)
{
	/* Take arguments and assign them to globally declared variables
	   so we can use them in the asm() construct */
	fd_g = fd;
	buf_g = buf;
	len_g = strlen(buf);

	asm volatile("mfence":::"memory");
	asm volatile  (	"mov	$1, %%rax\n\t"
			"mov	fd_g, %%rdi\n\t"
			"mov	buf_g, %%rsi\n\t"
			"mov 	len_g, %%rdx\n\t"
			"syscall\n\t"
			"mov    %%rax, ret_g\n\t"
			:
			:
			: "%rax", "%rdi", "%rsi", "%rdx"
			);
	
	return ret_g;
}

static ssize_t printf_asm(char *buf)
{
	return write_asm(1, buf);
}

static void syscall_tester(void)
{
	ssize_t ret;

	ret = printf_asm("Hello World from the assembler!!\n");
	printf("Returned from first syscall\n");
	printf("%zd values were printed\n\n", ret);

	ret = printf_asm("Hello once again from the assembler!!\n");
	printf("Returned from second syscall\n");
	printf("%zd values were printed\n\n", ret);

	//ret = write_asm(1, "sadf\n");
	//printf_asm("jahskjdhfashf\n");
}

int main(int argc, char** argv)
{
	syscall_tester();

	return 0;
}
