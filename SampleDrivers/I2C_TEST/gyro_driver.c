/****************************************
 * Quick test of I2C routines
 * Gyro Driver
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



int main(void)
{
  int i,j;
  int handle;
  unsigned char tval;
  //bmp085_coeff_t coeff;
  unsigned char *cbuf;
  unsigned char buf[10];
  unsigned char c, xl, xh, yl, yh, zh, zl;

  unsigned char oss;
  long ut;
  long up;
  long x, y, z;
  long b3, b4, b5, b6, b7;
  long p;
  long tx, ty, tz;
  float deg;
  float kpa;

	buf[0] = 0x20;
	buf[1] = 0x0F;

  //cbuf=(unsigned char *)&coeff;
  //Address = 0xD4 1101|0100
  handle=i2c_open(I2CBUS, 0x6A);

  i2c_write_byte(handle, 0x0F);
  i2c_read_byte(handle, &c);
  printf("\n Got %d", c);

  i2c_write_byte(handle, 0x20);
  delay_ms(6);
  i2c_write_byte(handle, 0x0F);

  i2c_write(handle, buf, 2);

  i2c_write_byte(handle, 0x20);
  i2c_read_byte(handle, &c);

  printf("\n Got %d", c);
while(1){
  //i2c_write_read(handle, 0xD4, )
   //Read X -axis registers
  i2c_write_byte(handle, 0x28);
  delay_ms(6);
  i2c_read_byte(handle, &xl);

  i2c_write_byte(handle, 0x29);
  delay_ms(6);
  i2c_read_byte(handle, &xh);


  //Read Y -axis registers
  i2c_write_byte(handle, 0x2A);
  delay_ms(6);
  i2c_read_byte(handle, &yl);

  i2c_write_byte(handle, 0x2B);
  delay_ms(6);
  i2c_read_byte(handle, &yh);

  //Read z -axis registers
  i2c_write_byte(handle, 0x2C);
  delay_ms(6);
  i2c_read_byte(handle, &zl);

  i2c_write_byte(handle, 0x2D);
  delay_ms(6);
  i2c_read_byte(handle, &zh);

  tx = (xh*256)+ xl;
  ty = (yh*256)+ yl;
  tz = (zh*256)+ zl;
  printf("\nx= %d y= %d z= %d", tx,ty,tz);
}

  return(0);
}
