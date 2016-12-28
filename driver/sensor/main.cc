#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

int twos_compliment(int val) {
    if (val >= 0x8000) {
        return -((65535 - val) + 1);
	} else {
        return val;
	}
}	

int main()
{
	int fd,ret;
	int raw_gyro_data[3];
	int raw_gyro_accel_data[3];

	fd = wiringPiI2CSetup(0x68);
	printf("setup return : %d\n", fd);

	while (1) {
		raw_gyro_data[0] = (wiringPiI2CReadReg8(fd, 0x3b) << 8) | wiringPiI2CReadReg8(fd, 0x3c);
		raw_gyro_data[1] = (wiringPiI2CReadReg8(fd, 0x3d) << 8) | wiringPiI2CReadReg8(fd, 0x3e);
		raw_gyro_data[2] = (wiringPiI2CReadReg8(fd, 0x3f) << 8) | wiringPiI2CReadReg8(fd, 0x40);
		printf("accel : %6d %6d %6d\n",
			   raw_gyro_data[0], raw_gyro_data[1], raw_gyro_data[2]);	
		delay(200);
	}

	return 0;
}
