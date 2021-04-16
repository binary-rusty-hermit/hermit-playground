#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

struct winsize {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
};

#define TIOCGWINSZ 0x00005413

/* needed for TCGETS */
#define NCCS	19

typedef unsigned char cc_t;
typedef uint32_t tcflag_t;

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_line;
	cc_t c_cc[NCCS];
};

#define TCGETS 0x00005401

volatile ssize_t fd;
volatile struct winsize *arg_winsize;
volatile struct termios *arg_termios;
volatile int cmd;
volatile ssize_t ret_g;


int main(int argc, char** argv)
{
	// Syscall IOCTL: 
	fd = 0;	// Dummy value
	cmd = TIOCGWINSZ;
	arg_winsize = malloc(sizeof(struct winsize));

	asm volatile  ( "mov    $16, %%rax\n\t"
                        "mov    fd, %%rdi\n\t"
			"mov 	cmd, %%rsi\n\t"
			"mov 	arg_winsize, %%rdx\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "%rdx"
                        );

	printf("%zd was returned from ioctl\n", ret_g);

	printf("%u, %u, %u, %u\n", arg_winsize->ws_row, arg_winsize->ws_col, arg_winsize->ws_xpixel, arg_winsize->ws_ypixel);

	cmd = TCGETS;
	arg_termios = malloc(sizeof(struct termios));
	
	 asm volatile  ( "mov    $16, %%rax\n\t"
                        "mov    fd, %%rdi\n\t"
                        "mov    cmd, %%rsi\n\t"
                        "mov    arg_termios, %%rdx\n\t"
                        "syscall\n\t"
                        "mov    %%rax, ret_g\n\t"
                        :
                        :
                        : "%rax", "%rdi", "%rsi", "%rdx"
                        );

        printf("%zd was returned from ioctl\n", ret_g);
	
	printf("%x, %x, %x, %x, %x\n", arg_termios->c_iflag, arg_termios->c_oflag, arg_termios->c_cflag, arg_termios->c_lflag, arg_termios->c_line);
	
	int i;
	for (i = 0; i < NCCS; i++) {
		printf("%x\n", arg_termios->c_cc[i]);
	}
	return 0;
}
