#include <stdio.h> 
#include <stddef.h> 
#include <time.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h>
#include <stdlib.h> 
#define BONEPATH	"/sys/devices/bone_capemgr.9/slots"
int pauseNanoSec(long nano); void pauseSec(int sec);
 /*
 * Usage: uart.c modulenumber baudrate
 * Example Use: uart.c 4 9600 */
 int main(int argc, char *argv[]) {
	//load the overlay for UART5
	FILE *uart;
	char buf[30] = "/dev/tty";
	
	//UART config using termios
	struct termios uart1,old;
	int fd;
	unsigned char buf2 = 0x55;
	if(argc != 3)
	{
		printf("Incorrect Usage: uart.c modulenumber baudrate\n");
		return 1;
	}
	printf("argv[0]: %s\n",argv[0]);
	printf("argv[1]: %s\n",argv[1]);
	printf("argv[2]: %s\n",argv[2]);
	uart = fopen(BONEPATH, "w");
	if(uart == NULL) printf("slots didn't open\n");
	fseek(uart,0,SEEK_SET);
	switch(atol(argv[1])){
		case 0:
			fprintf(uart, "BB-UART1");
			strcat(buf, "O0");
			break;
		case 1:
			fprintf(uart, "BB-UART2");
			strcat(buf, "O1");
			break;
		case 2:
			fprintf(uart, "BB-UART3");
			strcat(buf, "O2");
			break;
		case 3:
			fprintf(uart, "BB-UART4");
			strcat(buf, "O3");
			break;
		case 4:
			fprintf(uart, "BB-UART5");
			strcat(buf, "O4");
			break;
		default:
			printf("Uart modules are 0-4\n");
			return 1;
	}
	fflush(uart);
	fclose(uart);
	//open uart1 for tx/rx
	fd = open(buf, O_RDWR | O_NOCTTY);
	printf("Here1...");
	if(fd < 0) printf("port failed to open\n");
	//save current attributes
	tcgetattr(fd,&old);
	bzero(&uart1,sizeof(uart1));
	uart1.c_cflag = B4800 | CS8 | CLOCAL | CREAD;
	uart1.c_iflag = IGNPAR | ICRNL;
	uart1.c_oflag = 0;
	uart1.c_lflag = 0;
	uart1.c_cc[VTIME] = 0;
	uart1.c_cc[VMIN] = 1;
	//clean the line and set the attributes
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&uart1);
        printf("%s","About to send");
	while(1)
	{
		write(fd,&buf2,1);
		pauseNanoSec(5000000);
		if(read(fd,&buf2,1) > 0)
			printf("%c",buf2);
		pauseNanoSec(5000000);
	}
	close(fd);
	return 0;
}
int pauseNanoSec(long nano) {
	struct timespec tmr1,tmr2;
	tmr1.tv_sec = 0;
	tmr1.tv_nsec = nano;
	if(nanosleep(&tmr1,&tmr2) < 0)
	{
		printf("Nano second pause failed\n");
		return -1;
	}
}
void pauseSec(int sec) {
	time_t now,later;
	now = time(NULL);
	later = time(NULL);
	while((later - now) < (double)sec)
		later = time(NULL);
}
