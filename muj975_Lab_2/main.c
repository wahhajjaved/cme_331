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

#define GPIO_PORTF1_DATA_R (*((volatile unsigned long *)0x40025008))	 // p.654 and 662
#define GPIO_PORTF2_DATA_R (*((volatile unsigned long *)0x40025010))	 // p.654 and 662
#define GPIO_PORTF3_DATA_R (*((volatile unsigned long *)0x40025020))	 // p.654 and 662

/*** Timers ***/
#define SYSCTL_RCGCTIMER_R (*((volatile unsigned long *) (0x400FE000 + 0x604) )) //p. 338
#define TIMER0_CTL_R (*((volatile unsigned long *) (0x40030000 + 0x00C ))) // p. 737 control
#define TIMER0_CFG_R (*((volatile unsigned long *) 0x40030000 )) // p. 727 configuration
#define TIMER0_TAMR_R (*((volatile unsigned long *) (0x40030000 + 0x004 ))) // p. 729 prescale
#define TIMER0_IMR_R (*((volatile unsigned long *) (0x40030000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER0_TAILR_R (*((volatile unsigned long *) (0x40030000 + 0x028 ))) // p. 756 interval load
#define TIMER0_TAPR_R (*((volatile unsigned long *) (0x40030000 + 0x038 ))) // p. 760 mistake in lab manual. TAPS should be TAPR
#define TIMER0_TAV_R (*((volatile unsigned long *) (0x40030000 + 0x050 ))) // p. 766 Timer value
#define TIMER0_RIS_R (*((volatile unsigned long *) (0x40030000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER0_ICR_R (*((volatile unsigned long *) (0x40030000 + 0x024 ))) // p. 754 Interrupt clear


#define CLOCK_FREQUENCY_MS 16000 //16E6 ticks per second to 16000 ticks per ms
#define LED_TIMER_VALUE CLOCK_FREQUENCY_MS * 1000 //blink LEDs at 1Hz


//***************** Function Declarations *********************//
int timed_out(void);
void time_out_handler(void);
void init_gpio(void);
void set_led(char state);
void change_led_colour(void);
void toggle_led(void);

//***************** Global Variables *********************//
char led_colour = 'r';
int led_on = 0; //0 = off, 1 = on

int sw1_pressed = 0; // 0 = not pressed, 1 = pressed. sw1 pressed in the last 5 ms
int sw1_pressed_start_time = -1; //-1 = sw1 not yet pressed

//***************** Main *********************//

int main(void) {
	/*** Code here runs only once ***/
	init_gpio();

	time_out_handler();

	/*** Code here repeats forever ***/
	while (1) {
		change_led_colour();

		if (timed_out())
			time_out_handler();
	}
}

//***************** Function Definitions *********************//



void init_gpio(void) {
	volatile unsigned long delay_clk;
	SYSCTL_RCGC2_R |= 0x00000020; // Enable Clock Gating for PortF, p.340
	SYSCTL_RCGCTIMER_R |= 0x1; // Enable Clock Gating for PortF, p.338
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


	/* Timer initialization. Page 722 */
	//1. Clear TAEN bit in TIMER0_CTL_R
	TIMER0_CTL_R &= ~0x0;

	//2. Set TIMER0_CFG_R to 0x00000000
	TIMER0_CFG_R  = 0x0;

	//3. Write 0x2 into TnMR filed of TIMER0_TAMR_R
	TIMER0_TAMR_R |= 0x2;

	//4. Load start value into TIMER0_TAILR_R
	TIMER0_TAILR_R = 4294967296 - 1 ;

	//5. Disable interrupts using TIMER0_IMR_R
	TIMER0_IMR_R = 0x0;

	//6. Disable prescale using TIMER0_TAPR_R
	TIMER0_TAPR_R = 0x0;

	//7. Enable the timer
	TIMER0_CTL_R |= 0x1;


}

//Checks if a timer has timed out. Returns 1 if timed out
int timed_out(void) {
	int status = TIMER0_RIS_R & 0x1;
	return status == 1;
}

/* Run when the timer times out. Updates led_oon and resets the timer and clears the interrupt*/
void time_out_handler(void) {
	if (led_on == 0) {
		led_on = 1;
		set_led(led_colour);
	}
	else {
		led_on = 0;
		set_led('o');
	}

	TIMER0_ICR_R |= 0x1; //writing a 1 to the TATOCINT to clear the timer interrupt
	TIMER0_TAILR_R = LED_TIMER_VALUE;
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


/*Updates sw1_state global variable once in a given interval to debounce the button*/
void update_sw1_state(void) {
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

	if (led_on == 1)
		set_led(led_colour);
}


