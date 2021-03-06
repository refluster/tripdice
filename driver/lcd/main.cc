#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "img_util.h"

#define ROW 320
#define COL 240
#define SOFT_SPI
#define min(a, b) (((a) < (b))? (a): (b))

const int pin_CS[] = {5, 6, 13, 19, 26, 21};
const int pin_SDA = 10;
const int pin_SCL = 11;
const int pin_RST[] = {20, 16, 12, 25, 24, 23};

static int lcd_no = 0;

void send_data_spi(unsigned char dat) {
	int channel = 0;
	int length = 1;

#ifndef SOFT_SPI
	wiringPiSPIDataRW(channel, &dat, length) ;

#else
	unsigned char i;
	for(i = 0; i < 8; i++) {
		if ((dat & 0x80) != 0) {
			digitalWrite(pin_SDA, 1);
		}
		else{
			digitalWrite(pin_SDA, 0);
		}
		dat <<= 1;
		digitalWrite(pin_SCL, 0);
		digitalWrite(pin_SCL, 1);     
	}
#endif
}

void write_comm(unsigned int i) {
	digitalWrite(pin_CS[lcd_no], 0);
	send_data_spi(0x70);
	send_data_spi(i >> 8);
	send_data_spi(i);
	digitalWrite(pin_CS[lcd_no], 1);
}

void write_data(unsigned int i) {
	digitalWrite(pin_CS[lcd_no], 0);
	send_data_spi(0x72);
	send_data_spi(i >> 8);
	send_data_spi(i);
	digitalWrite(pin_CS[lcd_no], 1);
}

void LCD_CtrlWrite_ILI9325C(unsigned int com, unsigned int dat) {
	write_comm(com);
	write_data(dat);
}

void lcd_init(void) {
	for (lcd_no = 0; lcd_no < sizeof(pin_RST)/sizeof(pin_RST[0]); lcd_no++) {
		digitalWrite(pin_RST[lcd_no], 1);
	}

	delay(100);

	for (lcd_no = 0; lcd_no < sizeof(pin_RST)/sizeof(pin_RST[0]); lcd_no++) {
		digitalWrite(pin_RST[lcd_no] ,0);
	}
	delay(500);
 
	for (lcd_no = 0; lcd_no < sizeof(pin_RST)/sizeof(pin_RST[0]); lcd_no++) {
		digitalWrite(pin_RST[lcd_no], 1);
	}
	delay(500);

	for (lcd_no = 0; lcd_no < sizeof(pin_CS)/sizeof(pin_CS[0]); lcd_no++) {
		printf("init %d\n", lcd_no);

		LCD_CtrlWrite_ILI9325C(0x0001, 0x0000);     // set SS and SM bit
		LCD_CtrlWrite_ILI9325C(0x0002, 0x0400);     // set 1 line inversion
		LCD_CtrlWrite_ILI9325C(0x0003, 0x1090);     // set GRAM write direction and BGR=1.
		LCD_CtrlWrite_ILI9325C(0x0004, 0x0000);     // Resize register
		LCD_CtrlWrite_ILI9325C(0x0008, 0x0202);     // set the back porch and front porch
		LCD_CtrlWrite_ILI9325C(0x0009, 0x0000);     // set non-display area refresh cycle ISC[3:0]
		LCD_CtrlWrite_ILI9325C(0x000A, 0x0000);     // FMARK function
		LCD_CtrlWrite_ILI9325C(0x000C, 0x0000);     // RGB interface setting
		LCD_CtrlWrite_ILI9325C(0x000D, 0x0000);     // Frame marker Position
		LCD_CtrlWrite_ILI9325C(0x000F, 0x0000);     // RGB interface polarity

		//*************Power On sequence **************** //

		LCD_CtrlWrite_ILI9325C(0x0010, 0x0000);       // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_CtrlWrite_ILI9325C(0x0011, 0x0007);       // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_CtrlWrite_ILI9325C(0x0012, 0x0000);       // VREG1OUT voltage
		LCD_CtrlWrite_ILI9325C(0x0013, 0x0000);       // VDV[4:0] for VCOM amplitude
		LCD_CtrlWrite_ILI9325C(0x0007, 0x0001);
		delay(200);                             // Dis-charge capacitor power voltage
		LCD_CtrlWrite_ILI9325C(0x0010, 0x1690);       // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_CtrlWrite_ILI9325C(0x0011, 0x0227);       // Set DC1[2:0], DC0[2:0], VC[2:0]
		delay(50);                       // delay 50ms  
		LCD_CtrlWrite_ILI9325C(0x0012, 0x008C);       // External reference voltage= Vci;           
		delay(50);                       // delay 50ms
		LCD_CtrlWrite_ILI9325C(0x0013, 0x1500);       //VDV[4:0] for VCOM amplitude  0X1000
		LCD_CtrlWrite_ILI9325C(0x0029, 0x0004);       // VCM[5:0] for VCOMH      0X0009
		LCD_CtrlWrite_ILI9325C(0x002B, 0x000D);       //Set Frame Rate
		delay(50);                               // delay 50ms
		LCD_CtrlWrite_ILI9325C(0x0020, 0x0000);       // GRAM horizontal Address
		LCD_CtrlWrite_ILI9325C(0x0021, 0x0000);       // GRAM Vertical Address 
		// ----------- Adjust the Gamma   Curve ----------//
		LCD_CtrlWrite_ILI9325C(0x0030, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0031, 0x0607);
		LCD_CtrlWrite_ILI9325C(0x0032, 0x0305);
		LCD_CtrlWrite_ILI9325C(0x0035, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0036, 0x1604);
		LCD_CtrlWrite_ILI9325C(0x0037, 0x0204);
		LCD_CtrlWrite_ILI9325C(0x0038, 0x0001);
		LCD_CtrlWrite_ILI9325C(0x0039, 0x0707);
		LCD_CtrlWrite_ILI9325C(0x003C, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x003D, 0x000F);

		//------------------ Set GRAM area ---------------//

		LCD_CtrlWrite_ILI9325C(0x0050, 0x0000);      // Horizontal GRAM Start Address
		LCD_CtrlWrite_ILI9325C(0x0051, 0x00EF);      // Horizontal GRAM End Address
		LCD_CtrlWrite_ILI9325C(0x0052, 0x0000);      // Vertical GRAM Start Address
		LCD_CtrlWrite_ILI9325C(0x0053, 0x013F);      // Vertical GRAM Start Address
		LCD_CtrlWrite_ILI9325C(0x0060, 0xA700);      // Gate Scan Line
		LCD_CtrlWrite_ILI9325C(0x0061, 0x0001);      // NDL,VLE, REV
		LCD_CtrlWrite_ILI9325C(0x006A, 0x0000);      //  set scrolling line

		//-------------- Partial Display Control ---------//
		LCD_CtrlWrite_ILI9325C(0x0080, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0081, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0082, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0083, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0084, 0x0000);
		LCD_CtrlWrite_ILI9325C(0x0085, 0x0000);

		//-------------- Panel Control -------------------//
		LCD_CtrlWrite_ILI9325C(0x0090, 0x0010);
		LCD_CtrlWrite_ILI9325C(0x0092, 0x0600);
		LCD_CtrlWrite_ILI9325C(0x0007, 0x0133);      // 262K color and display ON
	}
}

void block_write(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) {
	write_comm(0x0050); write_data(Xstart);
	write_comm(0x0051); write_data(Xend);
	write_comm(0x0052); write_data(Ystart);
	write_comm(0x0053); write_data(Yend);
	write_comm(0x0020);
	write_data(Xstart);
	write_comm(0x0021);
	write_data(Ystart);
	write_comm(0x0022);
}

void display_color(unsigned int color) {
	unsigned int i, j;
	digitalWrite(pin_CS[lcd_no], 0);
	block_write(0, COL - 1, 0, ROW - 1);
	for(i = 0; i < ROW; i++) {
		for(j = 0; j < COL; j++) {
			write_data(color);
		}
	}
	digitalWrite(pin_CS[lcd_no], 1);
}

void display_gradation() {
	int i, j;
	int r, g, b;
	digitalWrite(pin_CS[lcd_no], 0);
	block_write(0, COL - 1, 0, ROW - 1);
	r = ROW/2 - 1;
	b = 0;
	for(i = 0; i < ROW; i++) {
		g = 0;
		for(j = 0; j < COL; j++) {
			int color = ((r / 5) << 11) | ((g / 5) << 5) | ((b / 5)) ;
			write_data(color);
			++g;
		}
		if (i < ROW/2) {
			--r;
		} else {
			++b;
		}
	}
	digitalWrite(pin_CS[lcd_no], 1);
}

void display_image(uint16_t *img) {
	unsigned int i, j;
	digitalWrite(pin_CS[lcd_no], 0);
	block_write(0, COL - 1, 0, ROW - 1);
	for(i = 0; i < ROW; i++) {
		for(j = 0; j < COL; j++) {
			int color = img[i*COL + j];
			write_data(color);
		}
	}
	digitalWrite(pin_CS[lcd_no], 1);
}

void gpio_setup() {
	int channel = 0;
	int speed = 1000000;

#ifndef SOFT_SPI
	if (wiringPiSPISetup(channel, speed) < 0) {
		printf("wiringPiSPISetup error \n");
		return;
	}
#endif

	// Initialize WiringPi
	if(wiringPiSetupGpio() == -1) {
		printf("wiringPi setup error");
		return;
	}

	for (int i = 0; i < sizeof(pin_CS)/sizeof(pin_CS[0]); i++) {
		pinMode(pin_CS[i], OUTPUT);
		pinMode(pin_RST[i], OUTPUT);
	}
#ifdef SOFT_SPI
	pinMode(pin_SDA, OUTPUT);
	pinMode(pin_SCL, OUTPUT);
#endif
}

int main(int argc, char **argv) {
	struct option longopts[] = {
		{ "init",      no_argument,       NULL, 0   },
		{ "on",        no_argument,       NULL, 1   },
		{ "off",       no_argument,       NULL, 2   },
		{ "update",    required_argument, NULL, 3   },
		{ "test",      required_argument, NULL, 4   },
		{ 0,           0,                 0,    0   },
		//no_argument
		//required_argument
	};

	int opt;
	int longindex;

	int f_init = 0;
	int f_on = 0;
	int f_off = 0;
	int f_update = 0;
	int f_test = 0;

	char *update_file = NULL;
	int test_no = 5;

	while ((opt = getopt_long(argc, argv, "", longopts, &longindex)) != -1) {
		switch (opt) {
		case 0: // init
			f_init = 1;
			break;
		case 1: // on
			f_on = 1;
			break;
		case 2: // off
			f_off = 1;
			break;
		case 3: // update
			f_update = 1;
			update_file = optarg;
			break;
		case 4: // test
			f_test = 1;
			test_no = min(atoi(optarg), 5);
			break;
		default:
			printf("error! \'%c\' \'%c\'\n", opt, optopt);
			return 1;
		}
	}

	gpio_setup();

	if (f_init) {
		printf("init lcd module\n");
		lcd_init();
	} else if (f_on) {
		printf("power on lcd module (not implemented)\n");
		// not implemented
	} else if (f_off) {
		printf("power off lcd module (not implemented)\n");
		// not impelemented
	} else if (f_update) {
		printf("update display from %s\n", (*update_file == '-') ? "stdin": update_file);
		uint16_t *img;
		if (*update_file == '-') {
			puts("read bin");
			img = (uint16_t*)malloc(sizeof(uint16_t)*320*240);
			fread(img, sizeof(uint16_t), 320*240, stdin);
		} else {
			img = read_jpeg_file(update_file);
		}
		if (optind + 1 > argc) {
			puts("lack of argument");
			return 2;
		}
		lcd_no = atoi(argv[optind]);
		display_image(img);		
	} else if (f_test) {
		printf("test lcd pattern %d\n", test_no);
		if (optind + 1 > argc) {
			puts("lack of argument");
			return 2;
		}
		lcd_no = atoi(argv[optind]);
		switch(test_no) {
		case 0:
			display_color(0xf800);
			break;
		case 1:
			display_color(0x07e0);
			break;
		case 2:
			display_color(0x001f);
			break;
		case 3:
			display_color(0xffff);
			break;
		case 4:
			display_gradation();
			break;
		case 5:
			display_color(0xf800);
			display_color(0x07e0);
			display_color(0x001f);
			display_color(0xffff);
			display_gradation();
			break;
		}
	}

	return 0;
}
