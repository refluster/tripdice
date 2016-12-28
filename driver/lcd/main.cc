#include <wiringPi.h>

int main(void) {
	int i;

	// Initialize WiringPi
	if(wiringPiSetupGpio() == -1) return 1;

	// Set GPIO18 pin to output mode
	pinMode(16, OUTPUT);

	// Repeat LED blinking 10 times
	for(i=0; i<10; i++){
		digitalWrite(16, 0);
		delay(950);
		digitalWrite(16, 1);
		delay(50);
	}

	// Turn off LED
	digitalWrite(16, 0);

	return 0;
}
