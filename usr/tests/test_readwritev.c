#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

volatile ssize_t fd;
volatile ssize_t iov_ptr;
volatile int vlen;
volatile ssize_t ret_g;
volatile char *buf;
volatile int flags;
volatile int mode;

struct iovec {
    void *iov_base;     
    size_t iov_len;     
};

typedef struct iovec iovec;

int main(int argc, char** argv)
{
	// open
	buf = "Hello_world.txt";
	flags = 0x0100 | 0x0002; // flags to create a file if it doesn't exist and read and write operations
	mode = 777; // Permissions of the file upon creation: read, write, execute for everyone?

	asm volatile  ( "mov    $2, %%rax\n\t"
                        "mov    buf, %%rdi\n\t"
                        "mov    flags, %%rsi\n\t"
                        "mov    mode, %%rdx\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "rdx"
                        );

	fd = ret_g;

	printf("%zd was returned from open\n\n", ret_g);

        // readv
	char buf_0[11];
	char buf_1[11];
	char buf_2[21];
	char buf_3[3];
	char buf_4[21];
	char buf_5[11];
	int iovcnt;
	iovec iov1[6];
	
	iov1[0].iov_base = buf_0;
	iov1[0].iov_len = 10;
	iov1[1].iov_base = buf_1;
	iov1[1].iov_len = 10;
	iov1[2].iov_base = buf_2;
	iov1[2].iov_len = 20;
	iov1[3].iov_base = buf_3;
        iov1[3].iov_len = 2;
	iov1[4].iov_base = buf_4;
        iov1[4].iov_len = 20;
	iov1[5].iov_base = buf_5;
        iov1[5].iov_len = 10;

	iovcnt = sizeof(iov1) / sizeof(struct iovec);	

        //fd = 0;	// Standard input
	iov_ptr = &iov1;
	vlen = iovcnt;

        asm volatile  ( "mov    $19, %%rax\n\t"
                        "mov    fd, %%rdi\n\t"
			"mov	iov_ptr, %%rsi\n\t"
			"mov	vlen, %%rdx\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "rdx"
                        );

	buf_0[11] = 0x0;
	buf_1[11] = 0x0;
	buf_2[21] = 0x0;
	buf_3[3] = 0x0;
	buf_4[21] = 0x0;
	buf_5[11] = 0x0;

	printf("\nRead from file: \nstring 1: %s\nstring 2: %s\nstring 3: %s\nstring 4: %s\nstring 5: %s\nstring 6: %s\n", buf_0, buf_1, buf_2, buf_3, buf_4, buf_5);
        printf("%zd was returned from readv\n\n", ret_g);

	// close
	asm volatile  ( "mov    $3, %%rax\n\t"
                        "mov    fd, %%rdi\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi"
                        );

	printf("%zd was returned from close\n\n", ret_g);

        // writev
	iovec iov[3];
	char *buf0 = "short string\n";
	char *buf1 = "This is a longer string\n";
	char *buf2 = "This is the longest string in this example\n";

	iov[0].iov_base = buf0;
	iov[0].iov_len = strlen(buf0);
	iov[1].iov_base = buf1;
	iov[1].iov_len = strlen(buf1);
	iov[2].iov_base = buf2;
	iov[2].iov_len = strlen(buf2);

	iovcnt = sizeof(iov) / sizeof(struct iovec);
	printf("return should be: %i\n", strlen(buf0) + strlen(buf1) + strlen(buf2));
        fd = 1; // Standard output
	iov_ptr = &iov;
	vlen = iovcnt;

	asm volatile  ( "mov    $20, %%rax\n\t"
                        "mov    fd, %%rdi\n\t"
                        "mov    iov_ptr, %%rsi\n\t"
                        "mov    vlen, %%rdx\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "rdx"
                        );


        printf("%zd was returned from writev\n\n", ret_g);


        return 0;
}

