#include "helpers.h"
#include "tm4c123gh6pm.h"


/*
* state = 'o' --> off
* state = 'r' --> red led on
* state = 'g' --> green led on
* state = 'b' --> blue led on
* state = 'w' --> white led on
*/
void set_led(char state) {
	switch (state) {
		case 'o':
			GPIO_PORTF_DATA_R &= ~0x02; //Turn off LED_R
			GPIO_PORTF_DATA_R &= ~0x04; //Turn off LED_B
			GPIO_PORTF_DATA_R &= ~0x08; //Turn off LED_G
			break;

		case 'r':
			GPIO_PORTF_DATA_R |= 0x02; //Turn on LED_R
			GPIO_PORTF_DATA_R &= ~0x04; //Turn off LED_B
			GPIO_PORTF_DATA_R &= ~0x08; //Turn off LED_G
			break;

		case 'b':
			GPIO_PORTF_DATA_R &= ~0x02; //Turn off LED_R
			GPIO_PORTF_DATA_R |= 0x04; //Turn on LED_B
			GPIO_PORTF_DATA_R &= ~0x08; //Turn off LED_G
			break;

		case 'g':
			GPIO_PORTF_DATA_R &= ~0x02; //Turn off LED_R
			GPIO_PORTF_DATA_R &= ~0x04; //Turn off LED_B
			GPIO_PORTF_DATA_R |= 0x08; //Turn on LED_G
			break;

		case 'w':
			GPIO_PORTF_DATA_R |= 0x02; //Turn on LED_R
			GPIO_PORTF_DATA_R |= 0x04; //Turn on LED_B
			GPIO_PORTF_DATA_R |= 0x08; //Turn on LED_G
			break;
	}
}



/*
Write the given string to the 4 7-segment LED displays.
output_string must be a null terminated string of size 4 or less
not counting the null terminator. Currently only supports
characters '0' - '9', '.', and ' '
*/
void write_7_segment_display(char *output_string) {
	volatile int i;
	//Display 0 at PA4
	GPIO_PORTB_DATA_R = char_to_display_data(output_string[0]);
	GPIO_PORTA_DATA_R |= 0x1 << 4;
	GPIO_PORTA_DATA_R &= ~(0x1 << 4);

	//Display 1 at PA5
	GPIO_PORTB_DATA_R = char_to_display_data(output_string[1]);
	GPIO_PORTA_DATA_R |= 0x1 << 5;
	i = GPIO_PORTA_DATA_R;
	GPIO_PORTA_DATA_R &= ~(0x1 << 5);

	//Display 2 at PA 6
	GPIO_PORTB_DATA_R = char_to_display_data(output_string[2]);
	GPIO_PORTA_DATA_R |= 0x1 << 6;
	GPIO_PORTA_DATA_R &= ~(0x1 << 6);

	//Display 3 at PA7
	GPIO_PORTB_DATA_R = char_to_display_data(output_string[3]);
	GPIO_PORTA_DATA_R |= 0x1 << 7;
	GPIO_PORTA_DATA_R &= ~(0x1 << 7);
}

int char_to_display_data(char c) {
		switch(c) {
			case '0':
				return DISPLAY_SEGMENT_0;
			case '1':
				return DISPLAY_SEGMENT_1;
			case '2':
				return DISPLAY_SEGMENT_2;
			case '3':
				return DISPLAY_SEGMENT_3;
			case '4':
				return DISPLAY_SEGMENT_4;
			case '5':
				return DISPLAY_SEGMENT_5;
			case '6':
				return DISPLAY_SEGMENT_6;
			case '7':
				return DISPLAY_SEGMENT_7;
			case '8':
				return DISPLAY_SEGMENT_8;
			case '9':
				return DISPLAY_SEGMENT_9;
			case '.':
				return DISPLAY_SEGMENT_DOT;
			case ' ':
				return DISPLAY_SEGMENT_OFF;
			default:
				return DISPLAY_SEGMENT_OFF;
		}
}