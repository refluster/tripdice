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

	raw_gyro_data[0] = wiringPiI2CReadReg16(fd, 0x43);
	raw_gyro_data[1] = wiringPiI2CReadReg16(fd, 0x45);
	raw_gyro_data[2] = wiringPiI2CReadReg16(fd, 0x47);

	printf("gyro_x : %d\n", raw_gyro_data[0]);
	printf("gyro_y : %d\n", raw_gyro_data[1]);
	printf("gyro_z : %d\n", raw_gyro_data[2]);

	return 0;
}
