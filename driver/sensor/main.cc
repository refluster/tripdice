#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

int main()
{
	int fd,ret;

	fd = wiringPiI2CSetup(0x68);
	printf("setup return : %d\n",fd);

	ret = wiringPiI2CReadReg8(fd, 0x75);
	printf("0x75 : %d\n",ret);

	ret = wiringPiI2CReadReg8(fd, 0x6b);
	printf("0x6b : %d\n",ret);
	return 0;
}
