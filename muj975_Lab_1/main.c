/*
Wahhaj Javed
muj975
*/
/*Code Template For TM4C123 Launchpad*/
//***************** Mapping Registers *********************//
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
// General-Purpose Input/Outputs (GPIOs) p.649
/**** Port F ****/
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))	 // p.662
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))	 // p.663
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *)0x40025420)) // p.671
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))	 // p.682
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *)0x40025510))	 // p.677


//***************** Function Declarations *********************//
void init_gpio(void);


void set_led(char state);
void delay(int miliseconds);
void change_led_colour(void);
void toggle_led(void);

void gpiodata_register_test(void);


//***************** Global Variables *********************//
char led_colour = 'r';

//***************** Main *********************//

int main(void) {
	/*** Code here runs only once ***/
	init_gpio();
	// gpiodata_register_test();

	/*** Code here repeats forever ***/
	while (1) {
		// toggle_led();

		change_led_colour();
		set_led(led_colour);
		delay(1000);
		set_led('o');
		delay(1000);
	}
}

//***************** Function Definitions *********************//
void init_gpio(void) {
	volatile unsigned long delay_clk;
	SYSCTL_RCGC2_R |= 0x00000020; // Enable Clock Gating for PortF, p.340
	delay_clk = SYSCTL_RCGC2_R;	  // Dummy operation to wait a few clock cycles
								  // See p.227 for more information.


	GPIO_PORTF_DEN_R |= 0x02; //set PF1 (LED_R) to digital
	GPIO_PORTF_DIR_R |= 0x02; //set PF1 (LED_R) direction to output
	GPIO_PORTF_AFSEL_R &= ~0x02; //disable PF1 (LED_R) alternate functions

	GPIO_PORTF_DEN_R |= 0x04; //set PF2 (LED_B) to digital
	GPIO_PORTF_DIR_R |= 0x04; //set PF2 (LED_B) direction to output
	GPIO_PORTF_AFSEL_R &= ~0x04; //disable PF2 (LED_B) alternate functions

	GPIO_PORTF_DEN_R |= 0x08; //set PF3 (LED_G) to digital
	GPIO_PORTF_DIR_R |= 0x08; //set PF3 (LED_G) direction to output
	GPIO_PORTF_AFSEL_R &= ~0x08; //disable PF3 (LED_G) alternate functions

	GPIO_PORTF_DEN_R |= 0x10; //set PF4 (SW1) to digital
	GPIO_PORTF_DIR_R &= ~0x10; //set PF4 (SW1) direction to input
	GPIO_PORTF_AFSEL_R &= ~0x10; //disable PF4 (SW1) alternate functions
	GPIO_PORTF_PUR_R |= 0x10; //set the pull up resistor on PF4 (SW1)

}

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

void delay(int miliseconds) {
	//clock speed of 16Mhz so 16E6 ticks per second or 16E3 ticks per ms
	//It takes multiple ticks to count
	int ticks_to_count = 7;
	int num_ticks = 16000 / ticks_to_count * miliseconds;
	while(num_ticks-- > 0) {}
}

void change_led_colour(void) {
	int sw1_state = (GPIO_PORTF_DATA_R >> 4) & 0x01;

	//PF4 will go to ground when the switch is pressed
	if(sw1_state == 0) {
		if(led_colour == 'r')
			led_colour = 'b';
		else if(led_colour == 'b')
			led_colour = 'g';
		else
			led_colour = 'r';
	}
}

/* Turn the LED on when SW1 is pressed */
void toggle_led(void) {
	int sw1_state = (GPIO_PORTF_DATA_R >> 4) & 0x01;

	//PF4 will go to ground when the switch is pressed
	if(sw1_state == 0) {
		set_led('w');
	}
	else {
		set_led('o');
	}
}

/* This function sets the values GPIODATA register using the built in bit masking */
void gpiodata_register_test(void) {
	/*

	0x40025000 = 100 0000 0000 0010 0101 0000 0000 0000
	0x400253FC = 100 0000 0000 0010 0101 0011 1111 1100

	SW2 	PF0 = 100 0000 0000 0010 0101 0000 0000 0100 = 0x4002 5004
	Red 	PF1 = 100 0000 0000 0010 0101 0000 0000 1000 = 0x4002 5008
	Blue 	PF2 = 100 0000 0000 0010 0101 0000 0001 0000 = 0x4002 5010
	Green 	PF3 = 100 0000 0000 0010 0101 0000 0010 0000 = 0x4002 5020
	SW1 	PF4 = 100 0000 0000 0010 0101 0000 0100 0000 = 0x4002 5040

	*/

	#define GPIO_PORTF1_DATA_R (*((volatile unsigned long *)0x40025008))	 // p.654 and 662
	#define GPIO_PORTF2_DATA_R (*((volatile unsigned long *)0x40025010))	 // p.654 and 662
	#define GPIO_PORTF3_DATA_R (*((volatile unsigned long *)0x40025020))	 // p.654 and 662

	while(1) {
		//blink red
		GPIO_PORTF1_DATA_R = 0xFF; //set PF1
		delay(500);
		GPIO_PORTF1_DATA_R = 0x00; //clear PF1
		delay(500);

		//blink green
		GPIO_PORTF3_DATA_R = 0xFF; //set PF3
		delay(500);
		GPIO_PORTF3_DATA_R = 0x00; //set PF3
		delay(500);

		//blink blue
		GPIO_PORTF2_DATA_R = 0xFF; //set PF2
		delay(500);
		GPIO_PORTF2_DATA_R = 0x00; //clear PF2
		delay(500);
	}
}



