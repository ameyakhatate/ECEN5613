#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B9600   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO1" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

main()
{
    int fd, c, res;
    struct termios oldtio, newtio;
    char buf[4];
    // Load the pin configuration
    int ret = system("echo BB-UART1  > /sys/devices/bone_capemgr.9/slots");
    /* Open modem device for reading and writing and not as controlling tty */

    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;	
    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    // NMEA command to ouput all sentences
    // Note that this code & format values in manual are hexadecimal
   // write(fd, "$PTNLSNM,273F,01*27\r\n", 21);
	printf("Reached before while\n");
    /* terminal settings done, now handle input*/
    while (TRUE) {     /* loop continuously */
        /*  read blocks program execution until a line terminating character is
            input, even if more than 255 chars are input. If the number
            of characters read is smaller than the number of chars available,
            subsequent reads will return the remaining chars. res will be set
            to the actual number of characters actually read */



        res = read(fd, buf, 6);


	//for(k=0;k<3;k++){
	//	write(fd, "B1234!", 6);
	//
	//	for(i = 0; i < 20000; i++){
	//		for(j =0; j< 20000; j++);
	//	}
	//	}

	//for(k=0;k<3;k++){
	//	write(fd, "A5678!",  6);
	//
	//	for(i = 0; i<20000; i++){
	//		for(j=0;j<20000;j++);
	//	}
	//}

        buf[res] = 0;             /* set end of string, so we can printf */
	 printf("%d\n", res);
	// printf("%s\n", buf);
	 if(buf[0] == 'C')
         	printf("%c\n", buf[1]);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
}
