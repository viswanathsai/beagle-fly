// itg3200.cpp
// demo of read itg3200 gyro

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

char filename[20];

// itg3200 init codes
char write_buf_1[2] = { 0x3E, 0x00 };
char write_buf_2[2] = { 0x15, 0x07 };
char write_buf_3[2] = { 0x16, 0x1E };
char write_buf_4[2] = { 0x17, 0x21 };		// enable interrupt
char write_buf_5[1] = { 0x1B };

char GYRO_XOUT_H[1] = { 0x1D };
char GYRO_XOUT_L[1] = { 0x1E };
char GYRO_YOUT_H[1] = { 0x1F };
char GYRO_YOUT_L[1] = { 0x20 };
char GYRO_ZOUT_H[1] = { 0x21 };
char GYRO_ZOUT_L[1] = { 0x22 };

char INT_STATUS[1] = {0x1a};

char buf[8];

int file;
int adapter_nr = 3; 
int i2c_addr = 0x69; // itg3200 I2C address 
int i;
int read_bytes = 1;

float GyroX, GyroY, GyroZ, GyroTemp;
int g_offx, g_offy, g_offz;
void i2c_bus_init() ;


/* I2C busses, u-boot are 0 - 2, userspace is 1 - 3 */

/* i2c_bus_init() open i2c bus 3 if available */
void i2c_bus_init() {
	sprintf(filename, "/dev/i2c-3");
	if ((file = open(filename, O_RDWR)) < 0) {
		printf("Failed to open the bus.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		if (file >= 0)
			close(file);
		exit(1);
	} else {
		printf("Opened /dev/i2c-%d\n", adapter_nr);
	}
}

// init itg3200
void i2c_3200_init() {
	i2c_addr = 0x69;

	if (ioctl(file, I2C_SLAVE, i2c_addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		if (file >= 0)
			close(file);
		exit(1);
	} else {
		printf("Comm to itg3200 is open.\n");
	}

	if (write(file, write_buf_1, 2) != 2) /* first init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);

	} else {
		printf("i2c send buf[1] success.\n");
	}

	if (write(file, write_buf_2, 2) != 2) /* second init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);
	} else {
		printf("i2c send buf[2] success.\n");
	}

	if (write(file, write_buf_3, 2) != 2) /* second init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);
	} else {
		printf("i2c send buf[3] success.\n");
	}

	if (write(file, write_buf_4, 2) != 2) /* second init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);
	} else {
		printf("i2c send buf[4] success.\n");
	}

}


unsigned char i2c_itg3200_read(char * command)
{
	unsigned char buf2[1] = {0};
	int rb = 1;
	if (write(file, command, 1) != 1) /* send to format data */
	{
		printf("I2C Send %x Failed\n", i2c_addr);
		close(file);
		exit(1);
	}
	// delay for adjusting reporting speed. Bad data without.
	usleep(100); // 100k uS = 0.1S, 1k uSec = 0.001S
	if (read(file, buf2, rb) != rb) {
		printf("I2C Send %x Failed\n", i2c_addr);
	}
	return buf2[0];
}

unsigned char a1;
unsigned char b1;
int itg3200_readX()
{
	a1 = i2c_itg3200_read(GYRO_XOUT_H);
	b1 = i2c_itg3200_read(GYRO_XOUT_L);
	short m = 0;
	unsigned char a2[2] = {b1, a1};
	memcpy(&m, a2, 2);
	return m;
}

int itg3200_readX2()
{
	unsigned char aa = a1;
	unsigned char bb = b1;
	return aa * 256 + bb;
}
int itg3200_readY()
{
	unsigned char a = i2c_itg3200_read(GYRO_YOUT_H);
	unsigned char b = i2c_itg3200_read(GYRO_YOUT_L);
	short m = 0;
	unsigned char a2[2] = {b, a};
	memcpy(&m, a2, 2);
	return m;
}

int itg3200_readZ()
{
	unsigned char a = i2c_itg3200_read(GYRO_ZOUT_H);
	unsigned char b = i2c_itg3200_read(GYRO_ZOUT_L);
	short m = 0;
	unsigned char a2[2] = {b, a};
	memcpy(&m, a2, 2);
	return m;
}


/* extract data */
/* joy_x, joy_y, accel_x, accel_y, accel_z,
 * bit[0], button Z, bit[1] Button C  pressed = low */
void chuk_data() {

	GyroX = ((buf[4] << 8) | buf[5]) - g_offx;
	GyroY = ((buf[2] << 8) | buf[3]) - g_offy;
	GyroZ = ((buf[6] << 8) | buf[7]) - g_offz;
	GyroTemp = (buf[0] << 8) | buf[1]; // temperature

}


// wait for data ready, ture for ready, false for timeout
bool WaitForData()
{
	for(int i = 0; i < 100; i ++)
	{
		unsigned char aa = i2c_itg3200_read(INT_STATUS);
		//printf("aa %d ", aa);
		if((aa & 0x1) == 0x1)
		{
			return true;
		}
		usleep(1);
		printf("not ready \r\n");
	}
	return false;
}

int main(int argc, void ** argv)
{
	i2c_bus_init();
	i2c_3200_init();
	int offsetX = 0;
	int offsetY = 0;
	int offsetZ = 0;
	int offsetX2= 0;
	int count = 100;
	// get offset, keep gyro stable
	for(int i = 0; i < count; i ++)
	{
		WaitForData();
		short x = itg3200_readX();
		short y = itg3200_readY();
		short z = itg3200_readZ();
		//short x2 = itg3200_readX2();
		offsetX = offsetX + x;
		offsetY = offsetY + y;
		offsetZ = offsetZ + z;
		//offsetX2 = offsetX2 + x2;
	}
	offsetX = offsetX / count;
	offsetY = offsetY / count;
	offsetZ = offsetZ / count;
	//offsetX2 = offsetX2 /count;
	printf("offset  X: %8d  Y: %8d Z: %8d  X2: %8d \r\n", offsetX, offsetY, offsetZ, offsetX2);

	int i  = 0;
	while(1)
	{
		i++;
		WaitForData();
		int x = itg3200_readX() - offsetX;
		int y = itg3200_readY() - offsetY;
		int z = itg3200_readZ() - offsetZ;
		if(i / 20 * 20 == i)
		{
			printf("%8d %8d %8d %8d \r\n", i, x, y, z);
		}
		usleep(10000);
	}
	
	return 0;
}