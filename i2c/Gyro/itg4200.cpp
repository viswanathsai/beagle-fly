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
#include <iostream>
#include <time.h>
char filename[20];
#ifdef ITG3200
// itg3200 init codes
// char write_buf_1[2] = { 0x3E, 0x00 };
// char write_buf_2[2] = { 0x15, 0x07 };
// char write_buf_3[2] = { 0x16, 0x1E };
// char write_buf_4[2] = { 0x17, 0x21 };		// enable interrupt
// char write_buf_5[1] = { 0x1B };

// char GYRO_XOUT_H[1] = { 0x1D };
// char GYRO_XOUT_L[1] = { 0x1E };
// char GYRO_YOUT_H[1] = { 0x1F };
// char GYRO_YOUT_L[1] = { 0x20 };
// char GYRO_ZOUT_H[1] = { 0x21 };
// char GYRO_ZOUT_L[1] = { 0x22 };

// char INT_STATUS[1] = {0x1a};
#else // itg4200
//char write_buf_1[2] = { 0x20, 0x8F };
char write_buf_1[2] = { 0x20, 0xCF };
char write_buf_2[2] = { 0x24, 0x02 };
char write_buf_3[3] = { 0x23, 0x00 };

char GYRO_XOUT_H[1] = { 0x29 };
char GYRO_XOUT_L[1] = { 0x28 };
char GYRO_YOUT_H[1] = { 0x2B };
char GYRO_YOUT_L[1] = { 0x2A };
char GYRO_ZOUT_H[1] = { 0x2D };
char GYRO_ZOUT_L[1] = { 0x2C };

char INT_STATUS[1] = {0x1a};
#endif

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
	sleep(0.1);
	if (write(file, write_buf_2, 2) != 2) /* second init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);
	} else {
		sleep(0.1);
		printf("i2c send buf[2] success.\n");
	}

	if (write(file, write_buf_3, 2) != 2) /* second init string */
	{
		printf("i2c send %x failed\n", i2c_addr);
		close(file);
		exit(1);
	} else {
		sleep(0.1);	
		printf("i2c send buf[3] success.\n");
	}

	// if (write(file, write_buf_4, 2) != 2) /* second init string */
	// {
		// printf("i2c send %x failed\n", i2c_addr);
		// close(file);
		// exit(1);
	// } else {
		// printf("i2c send buf[4] success.\n");
	// }

}
class gyroValue
{
	public:
	short x;
	short y;
	short z;
};
gyroValue i2c_fastRead()
{
	unsigned char buf2[7] = {0,0,0,0,0,0,0};
	int rb = 7;
//	unsigned char buf1[6] = {0x28,0x29,0x2A,0x2B,0x2C,0x2D};
		unsigned char buf1[1] = {0xA7};//,0x29,0x2A,0x2B,0x2C,0x2D};
	if (write(file, buf1, 1) != 1) /* send to format data */
	{
		printf("I2C Send %x Failed\n", i2c_addr);
		close(file);
		exit(1);
	}
	usleep(1000);
	// delay for adjusting reporting speed. Bad data without.
	if (read(file, buf2, rb) != rb) {
		printf("I2C Send %x Failed\n", i2c_addr);
	}
	gyroValue v;
	if((buf2[0] & 0x8 )== 0x8)
	{
		short x, y, z;
		unsigned char x2[2] = {buf2[1], buf2[2]};
		memcpy(&x, x2, 2);
		unsigned char y2[2] = {buf2[3], buf2[4]};
		memcpy(&y, y2, 2);
		unsigned char z2[2] = {buf2[5], buf2[6]};
		memcpy(&z, z2, 2);
		v.x = x;
		v.y = y;
		v.z = z;
	
	}
	else
	{
		//printf(" Wrong data %d\n", buf2[0]);
	}
	
	return v;
	//printf("%d %d %d \n", x, y, z);
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

int failcount = 0;
// wait for data ready, ture for ready, false for timeout
bool WaitForData()
{
	for(int i = 0; i < 100; i ++)
	{
		unsigned char aa = i2c_itg3200_read(INT_STATUS);
		//printf("aa %d ", aa);
		if((aa & 0x8) == 0x8)
		{
			return true;
		}
		else
		{
			usleep(100);
			printf("not ready \r\n");
			failcount ++;
		}
	}

	return false;
}

 timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
 
long diffMille(timespec start, timespec end)	// return elapse in milli second
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp.tv_nsec / 50000 + temp.tv_sec * 20000;
}

unsigned int min(unsigned int a, unsigned int b)
{
	if (a < b)
		return a;
	return b;
}


static timespec startTime;
static long mic = 0;
long  micros()
{
	// mic = mic + 1000;
	// return mic;
	timespec now;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
	return diffMille(startTime, now);
}

int main(int argc, void ** argv)
{
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	long start = 0;
	long cur = 0;
	i2c_bus_init();
	i2c_3200_init();
	int offsetX = 0;
	int offsetY = 0;
	int offsetZ = 0;
	int offsetX2= 0;
	int count = 100;
	// get offset, keep gyro stable
	short x,y,z;
	for(int i = 0; i < count; i ++)
	{
		usleep(1000);
		//WaitForData();

		
		// short x = itg3200_readX();
		// short y = itg3200_readY();
		// short z = itg3200_readZ();
		//short x2 = itg3200_readX2();
		gyroValue v = 		i2c_fastRead();
		// usleep(1000);
		offsetX = offsetX + v.x;
		offsetY = offsetY + v.y;
		offsetZ = offsetZ + v.z;
		//offsetX2 = offsetX2 + x2;
	}
	offsetX = offsetX / count;
	offsetY = offsetY / count;
	offsetZ = offsetZ / count;
	//offsetX2 = offsetX2 /count;
	printf("offset  X: %8d  Y: %8d Z: %8d  X2: %8d \r\n", offsetX, offsetY, offsetZ, offsetX2);
	int j = 0;
	int i  = 0;
	while(i < 10000)
	{
		
		i++;
		j ++;
		usleep(100);
		//WaitForData();
		
		// // //WaitForData();
		// int x = itg3200_readX() - offsetX;
		// int y = itg3200_readY() - offsetY;
		// int z = itg3200_readZ() - offsetZ;
		gyroValue v = 		i2c_fastRead();
		x = v.x - offsetX;
		y = v.y - offsetY;
		z = v.z - offsetZ;
		cur = micros();
		if(cur - start > 1000)
		{
			printf("%8d %8d %8d %8d \r\n", j, x , y , z );
			start = cur;
			j = 0;
		}
		// i2c_fastRead(x,y,z);
		// if(i / 20 * 20 == i)
		// {
			// printf("%8d %8d %8d %8d \r\n", i, x -offsetX, y - offsetY, z - offsetX2);
		// }
		//usleep(1000);
		
	}
	printf("fail count %d\n", failcount);
	
	return 0;
}