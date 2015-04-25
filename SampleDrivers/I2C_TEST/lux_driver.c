/****************************************
 * Quick test of I2C routines
 * Temp/pressure measuring using BMP085
 ****************************************/

#include <stdio.h>
#include "i2cfunc.h"
#include "i2cfunc.c"

// we will use I2C2 which is enumerated as 1 on the BBB
#define I2CBUS 1

// set to 1 to print out the intermediate calculations
#define DBG_PRINT 0
// set to 1 to use the example values in the datasheet
// rather than the values retrieved via i2c
#define ALG_TEST 0

typedef struct bmp085_coeff_s
{
  short int ac1;
  short int ac2;
  short int ac3;
  unsigned short int ac4;
  unsigned short int ac5;
  unsigned short int ac6;
  short int b1;
  short int b2;
  short int mb;
  short int mc;
  short int md;
} bmp085_coeff_t;

const unsigned int conversion_delay[4]={6, 10, 15, 30};

int
main(void)
{
  int i,j;
  int handle;
  unsigned char tval;
  bmp085_coeff_t coeff;
  unsigned char *cbuf;
  unsigned char buf[10];
  unsigned char  DataHigh, DataLow,c;
  unsigned int  channel0;
  unsigned char oss;
  long ut;
  long up;
  long x1, x2, x3;
  long b3, b4, b5, b6, b7;
  long p;
  long t;
  float deg;
  float kpa;

  //cbuf=(unsigned char *)&coeff;
  //Address = 0x39
  handle=i2c_open(I2CBUS, 0x39);

  i2c_write_byte(handle, 0x80);
  delay_ms(6);
  i2c_write_byte(handle, 0x03);
  delay_ms(6);
  i2c_read_byte(handle, &c);

 // printf("\n  GOT %d ",c);


  //Command = 0x8C
  while(1){
  i2c_write_byte(handle, 0x8C);
  delay_ms(6);
  i2c_read_byte(handle, &DataLow);
  delay_ms(6);
 // printf("\nDATA LOW BYTE:  %d", c);

	i2c_write_byte(handle, 0x8D);
	delay_ms(6);
  	i2c_read_byte(handle, &DataHigh);

  //printf("\nDATA HIGH BYTE: %d", c);

  // print out the results
	channel0 = 256*DataHigh + DataLow;
	printf("\nReading now: %d",channel0);
  for(i=0;i<200;i++){
	  for(j=0;j<200;j++);
  }

  }

  return(0);
}



