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
#define GPIO_PORTF_LOCK_R (*((volatile unsigned long *)0x40025520))	 // p.684
#define GPIO_PORTF_CR_R (*((volatile unsigned long *)0x40025524))	 // p.685



/*** Timers ***/
//Timer 0 for LED control
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

//Timer 1 for SW1 debounce
#define TIMER1_CTL_R (*((volatile unsigned long *) (0x40031000 + 0x00C ))) // p. 737 control
#define TIMER1_CFG_R (*((volatile unsigned long *) 0x40031000 )) // p. 727 configuration
#define TIMER1_TAMR_R (*((volatile unsigned long *) (0x40031000 + 0x004 ))) // p. 729 prescale
#define TIMER1_IMR_R (*((volatile unsigned long *) (0x40031000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER1_TAILR_R (*((volatile unsigned long *) (0x40031000 + 0x028 ))) // p. 756 interval load
#define TIMER1_TAPR_R (*((volatile unsigned long *) (0x40031000 + 0x038 ))) // p. 760 mistake in lab manual. TAPS should be TAPR
#define TIMER1_TAV_R (*((volatile unsigned long *) (0x40031000 + 0x050 ))) // p. 766 Timer value
#define TIMER1_RIS_R (*((volatile unsigned long *) (0x40031000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER1_ICR_R (*((volatile unsigned long *) (0x40031000 + 0x024 ))) // p. 754 Interrupt clear

//Timer 2 for SW2 debounce
#define TIMER2_CTL_R (*((volatile unsigned long *) (0x40032000 + 0x00C ))) // p. 737 control
#define TIMER2_CFG_R (*((volatile unsigned long *) 0x40032000 )) // p. 727 configuration
#define TIMER2_TAMR_R (*((volatile unsigned long *) (0x40032000 + 0x004 ))) // p. 729 prescale
#define TIMER2_IMR_R (*((volatile unsigned long *) (0x40032000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER2_TAILR_R (*((volatile unsigned long *) (0x40032000 + 0x028 ))) // p. 756 interval load
#define TIMER2_TAPR_R (*((volatile unsigned long *) (0x40032000 + 0x038 ))) // p. 760 mistake in lab manual. TAPS should be TAPR
#define TIMER2_TAV_R (*((volatile unsigned long *) (0x40032000 + 0x050 ))) // p. 766 Timer value
#define TIMER2_RIS_R (*((volatile unsigned long *) (0x40032000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER2_ICR_R (*((volatile unsigned long *) (0x40032000 + 0x024 ))) // p. 754 Interrupt clear




#define CLOCK_FREQUENCY_MS 16000 //16E6 ticks per second to 16000 ticks per ms
#define LED_TIMER_VALUE CLOCK_FREQUENCY_MS * 500 //blink LEDs at 1Hz
#define SW_TIMER_VALUE CLOCK_FREQUENCY_MS * 5 //5ms clock for debouncing


//***************** Function Declarations *********************//
void init_gpio(void);
void led_time_out_handler(void);
void check_sw1(void);
void check_sw2(void);
void set_led(char state);
void change_led_colour(void);
void toggle_led(void);

//***************** Global Variables *********************//
char led_colour = 'r';
int led_on = 0; //0 = off, 1 = on

int sw1_pressed = 0; // 0 = not pressed, 1 = pressed. sw1 pressed in the last 5 ms
int sw2_pressed = 0; // 0 = not pressed, 1 = pressed. sw2 pressed in the last 5 ms

int led_blinking = 1; //0 means led is not blinking, 1 means led is on

//***************** Main *********************//

int main(void) {
	/*** Code here runs only once ***/
	init_gpio();

	led_time_out_handler();

	/*** Code here repeats forever ***/
	while (1) {
		check_sw1();
		check_sw2();
		if(led_blinking == 0)
			set_led(led_colour);
		else if (TIMER0_RIS_R & 0x1)
			led_time_out_handler();
	}
}

//***************** Function Definitions *********************//

void init_gpio(void) {
	volatile unsigned long delay_clk;
	SYSCTL_RCGC2_R |= 0x00000020; // Enable Clock Gating for PortF, p.340
	SYSCTL_RCGCTIMER_R |= 0x6F; // Enable Clock Gating for Timers 0, 1, and 2, p.338
	delay_clk = SYSCTL_RCGC2_R;	  // Dummy operation to wait a few clock cycles
								  // See p.227 for more information.

	(*((volatile unsigned long *)0x4002551C))

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


	/* Timer 0 initialization. Page 722 */
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

	/* Timer 1 initialization. Page 722 */
	TIMER1_CTL_R &= ~0x0;
	TIMER1_CFG_R  = 0x0;
	TIMER1_TAMR_R |= 0x12; //0x2 in bit field 1:0 for preriodic mode and 0x1 in bit field 4 to count up. 0001 0010
	TIMER1_TAILR_R = 4294967296 - 1 ;
	TIMER1_IMR_R = 0x0;
	TIMER1_TAPR_R = 0x0;
	TIMER1_CTL_R |= 0x1;

	/* Timer 2 initialization. Page 722 */
	TIMER2_CTL_R &= ~0x0;
	TIMER2_CFG_R  = 0x0;
	TIMER2_TAMR_R |= 0x12; //0x2 in bit field 1:0 for preriodic mode and 0x1 in bit field 4 to count up. 0001 0010
	TIMER2_TAILR_R = 4294967296 - 1 ;
	TIMER2_IMR_R = 0x0;
	TIMER2_TAPR_R = 0x0;
	TIMER2_CTL_R |= 0x1;


	/*** SW2 Initialization. SW0 is connected to PF0 ***/
	//1. Unlock PF0 by writing 0x4C4F434B to GPIO_PORTF_LOCK_R (p.684)
	GPIO_PORTF_LOCK_R = 0x4C4F434B;

	//2. Enable writes to PF0 registers by writing 1 to bits 7:0 of GPIO_PORTF_CR_R (p.685)
	GPIO_PORTF_CR_R |= 0x0F;

	//3. Configure PF0 same as PF0
	GPIO_PORTF_DEN_R |= 0x01; //set PF0 (SW2) to digital
	GPIO_PORTF_DIR_R &= ~0x01; //set PF0 (SW2) direction to input
	GPIO_PORTF_AFSEL_R &= ~0x01; //disable PF0 (SW2) alternate functions
	GPIO_PORTF_PUR_R |= 0x01; //set the pull up resistor on PF0 (SW2)

}


/* Run when the timer times out. Updates led_oon and resets the timer and clears the interrupt*/
void led_time_out_handler(void) {
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

/*Detects SW1 state while debouncing it*/
void check_sw1(void) {
	/*
	Debouncing Algorithm
	The first push of the button is considered a press
	Until the button is released, all signals from the button are ignored
	After the first button release is detected, a timer is started
	Until the timer times out, all signals from the button are ignored
	*/

	//If the timer hasn't timed out yet, then ignore the button
	if (TIMER1_TAV_R < SW_TIMER_VALUE)
		return;

	int sw1_state = (GPIO_PORTF_DATA_R >> 4) & 0x01;

	// Initial state. Button is not pressed.
	//When the button is pressed, the button press is recorded and the SW1 handler is run
	if (sw1_pressed == 0 && sw1_state == 0) {
		sw1_pressed = 1;
		change_led_colour();
		return;
	}

	//When the button has been previously pressed, need to detect that it was released
	//When the button is released, the timer is restarted so all signals from the
	//button can be ignored until it times out
	if (sw1_pressed == 1 && sw1_state == 1) {
		sw1_pressed = 0;
		TIMER1_TAV_R = 0;
		return;
	}
}

/* Detects SW2 state while debouncing it
	Same code as check_sw1(), except for sw2
*/
void check_sw2(void) {

	if (TIMER2_TAV_R < SW_TIMER_VALUE)
		return;

	int sw2_state = GPIO_PORTF_DATA_R & 0x01;

	if (sw2_pressed == 0 && sw2_state == 0) {
		sw2_pressed = 1;
		if (led_blinking == 0)
			led_blinking = 1;
		else
			led_blinking = 0;

		return;
	}

	if (sw2_pressed == 1 && sw2_state == 1) {
		sw2_pressed = 0;
		TIMER2_TAV_R = 0;
		return;
	}

	return;
}


/*Moves the led to the next colour in rbg order*/
void change_led_colour(void) {
	if(led_colour == 'r')
		led_colour = 'b';
	else if(led_colour == 'b')
		led_colour = 'g';
	else
		led_colour = 'r';

	if (led_on == 1)
		set_led(led_colour);
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





