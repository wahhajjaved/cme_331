/*
Wahhaj Javed
muj975
*/

// #define DEBUG

#include "logging.h"
#include <stdio.h>

/*Code Template For TM4C123 Launchpad*/
//***************** Mapping Registers *********************//
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
// General-Purpose Input/Outputs (GPIOs) p.649

/**** NVIC ****/
#define NVIC_EN0_R (*((volatile unsigned long *)0xE000E100))
#define NVIC_EN1_R (*((volatile unsigned long *)0xE000E104))
#define NVIC_EN2_R (*((volatile unsigned long *)0xE000E108))
#define NVIC_EN3_R (*((volatile unsigned long *)0xE000E10C))
#define NVIC_EN4_R (*((volatile unsigned long *)0xE000E110))


/**** Port A ****/
#define GPIO_PORTA_DATA_R (*((volatile unsigned long *)0x400043FC))	 // p.662
#define GPIO_PORTA_DIR_R (*((volatile unsigned long *)0x40004400))	 // p.663
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long *)0x40004420)) // p.671
#define GPIO_PORTA_DEN_R (*((volatile unsigned long *)0x4000451C))	 // p.682
#define GPIO_PORTA_PUR_R (*((volatile unsigned long *)0x40004510))	 // p.677

/**** Port B ****/
#define GPIO_PORTB_DATA_R (*((volatile unsigned long *)0x400053FC))	 // p.662
#define GPIO_PORTB_DIR_R (*((volatile unsigned long *)0x40005400))	 // p.663
#define GPIO_PORTB_AFSEL_R (*((volatile unsigned long *)0x40005420)) // p.671
#define GPIO_PORTB_DEN_R (*((volatile unsigned long *)0x4000551C))	 // p.682
#define GPIO_PORTB_PUR_R (*((volatile unsigned long *)0x40005510))	 // p.677

/**** Port F ****/
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))	 // p.662
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))	 // p.663
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *)0x40025420)) // p.671
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))	 // p.682
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *)0x40025510))	 // p.677
#define GPIO_PORTF_LOCK_R (*((volatile unsigned long *)0x40025520))	 // p.684
#define GPIO_PORTF_CR_R (*((volatile unsigned long *)0x40025524))	 // p.685
#define GPIO_PORTF_IS_R (*((volatile unsigned long *)0x40025404))	 // p.664
#define GPIO_PORTF_IEV_R (*((volatile unsigned long *)0x4002540C))	 // p.666
#define GPIO_PORTF_IM_R (*((volatile unsigned long *)0x40025410))	 // p.667
#define GPIO_PORTF_MIS_R (*((volatile unsigned long *)0x40025418))	 // p.669
#define GPIO_PORTF_ICR_R (*((volatile unsigned long *)0x4002541C))	 // p.670




/*** Timers ***/
//Timer 0 for LED control
#define SYSCTL_RCGCTIMER_R (*((volatile unsigned long *) (0x400FE000 + 0x604) )) //p. 338
#define TIMER0_CTL_R (*((volatile unsigned long *) (0x40030000 + 0x00C ))) // p. 737 control
#define TIMER0_CFG_R (*((volatile unsigned long *) 0x40030000 )) // p. 727 configuration
#define TIMER0_TAMR_R (*((volatile unsigned long *) (0x40030000 + 0x004 ))) // p. 729 timer mode
#define TIMER0_IMR_R (*((volatile unsigned long *) (0x40030000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER0_TAILR_R (*((volatile unsigned long *) (0x40030000 + 0x028 ))) // p. 756 interval load
#define TIMER0_TAPR_R (*((volatile unsigned long *) (0x40030000 + 0x038 ))) // p. 760 prescale
#define TIMER0_TAV_R (*((volatile unsigned long *) (0x40030000 + 0x050 ))) // p. 766 Timer value
#define TIMER0_RIS_R (*((volatile unsigned long *) (0x40030000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER0_ICR_R (*((volatile unsigned long *) (0x40030000 + 0x024 ))) // p. 754 Interrupt clear

//Timer 1 for SW1 debounce
#define TIMER1_CTL_R (*((volatile unsigned long *) (0x40031000 + 0x00C ))) // p. 737 control
#define TIMER1_CFG_R (*((volatile unsigned long *) 0x40031000 )) // p. 727 configuration
#define TIMER1_TAMR_R (*((volatile unsigned long *) (0x40031000 + 0x004 ))) // p. 729 timer mode
#define TIMER1_IMR_R (*((volatile unsigned long *) (0x40031000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER1_TAILR_R (*((volatile unsigned long *) (0x40031000 + 0x028 ))) // p. 756 interval load
#define TIMER1_TAPR_R (*((volatile unsigned long *) (0x40031000 + 0x038 ))) // p. 760 prescale
#define TIMER1_TAV_R (*((volatile unsigned long *) (0x40031000 + 0x050 ))) // p. 766 Timer value
#define TIMER1_RIS_R (*((volatile unsigned long *) (0x40031000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER1_ICR_R (*((volatile unsigned long *) (0x40031000 + 0x024 ))) // p. 754 Interrupt clear

//Timer 2 for SW2 debounce
#define TIMER2_CTL_R (*((volatile unsigned long *) (0x40032000 + 0x00C ))) // p. 737 control
#define TIMER2_CFG_R (*((volatile unsigned long *) 0x40032000 )) // p. 727 configuration
#define TIMER2_TAMR_R (*((volatile unsigned long *) (0x40032000 + 0x004 ))) // p. 729 timer mode
#define TIMER2_IMR_R (*((volatile unsigned long *) (0x40032000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER2_TAILR_R (*((volatile unsigned long *) (0x40032000 + 0x028 ))) // p. 756 interval load
#define TIMER2_TAPR_R (*((volatile unsigned long *) (0x40032000 + 0x038 ))) // p. 760 prescale
#define TIMER2_TAV_R (*((volatile unsigned long *) (0x40032000 + 0x050 ))) // p. 766 Timer value
#define TIMER2_RIS_R (*((volatile unsigned long *) (0x40032000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER2_ICR_R (*((volatile unsigned long *) (0x40032000 + 0x024 ))) // p. 754 Interrupt clear

//Timer 3 for 7-segment display counter
#define TIMER3_CTL_R (*((volatile unsigned long *) (0x40033000 + 0x00C ))) // p. 737 control
#define TIMER3_CFG_R (*((volatile unsigned long *) 0x40033000 )) // p. 727 configuration
#define TIMER3_TAMR_R (*((volatile unsigned long *) (0x40033000 + 0x004 ))) // p. 729 timer mode
#define TIMER3_IMR_R (*((volatile unsigned long *) (0x40033000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER3_TAILR_R (*((volatile unsigned long *) (0x40033000 + 0x028 ))) // p. 756 interval load
#define TIMER3_TAPR_R (*((volatile unsigned long *) (0x40033000 + 0x038 ))) // p. 760 prescale
#define TIMER3_TAV_R (*((volatile unsigned long *) (0x40033000 + 0x050 ))) // p. 766 Timer value
#define TIMER3_ICR_R (*((volatile unsigned long *) (0x40033000 + 0x024 ))) // p. 754 Interrupt clear

//Timer 4 for square() profiling
#define TIMER4_CTL_R (*((volatile unsigned long *) (0x40034000 + 0x00C ))) // p. 737 control
#define TIMER4_CFG_R (*((volatile unsigned long *) 0x40034000 )) // p. 727 configuration
#define TIMER4_TAMR_R (*((volatile unsigned long *) (0x40034000 + 0x004 ))) // p. 729 timer mode
#define TIMER4_IMR_R (*((volatile unsigned long *) (0x40034000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER4_TAILR_R (*((volatile unsigned long *) (0x40034000 + 0x028 ))) // p. 756 interval load
#define TIMER4_TAPR_R (*((volatile unsigned long *) (0x40034000 + 0x038 ))) // p. 760 prescale
#define TIMER4_TAV_R (*((volatile unsigned long *) (0x40034000 + 0x050 ))) // p. 766 Timer value
#define TIMER4_ICR_R (*((volatile unsigned long *) (0x40034000 + 0x024 ))) // p. 754 Interrupt clear


#define CLOCK_FREQUENCY_MS 16000 //16E6 ticks per second to 16000 ticks per ms
#define LED_TIMER_VALUE CLOCK_FREQUENCY_MS * 500 //blink LEDs at 1Hz
#define SW_TIMER_VALUE CLOCK_FREQUENCY_MS * 30 //30ms clock for debouncing
#define SSD_TIMER_VALUE CLOCK_FREQUENCY_MS * 1000 //Write to seven segment display at 1Hz

//look up for the seven segment display
#define DISPLAY_SEGMENT_OFF 0xFF
#define DISPLAY_SEGMENT_0 0x81
#define DISPLAY_SEGMENT_1 0xE7
#define DISPLAY_SEGMENT_2 0x92
#define DISPLAY_SEGMENT_3 0xC2
#define DISPLAY_SEGMENT_4 0xE4
#define DISPLAY_SEGMENT_5 0xC8
#define DISPLAY_SEGMENT_6 0x8C
#define DISPLAY_SEGMENT_7 0xE3
#define DISPLAY_SEGMENT_8 0x80
#define DISPLAY_SEGMENT_9 0xE0
#define DISPLAY_SEGMENT_DOT 0x7F


//***************** Function Declarations *********************//
void init_gpio(void);
void init_interrupts(void);
void led_time_out_handler(void);
void check_sw1(void);
void check_sw2(void);
void set_led(char state);
void change_led_colour(void);
void toggle_led(void);
void timer0_handler(void);
void write_7_segment_display(char *output_string);
int char_to_display_data(char c);
void sw1_handler(void);
void timer1_handler(void);
void timer2_handler(void);
void timer3_handler(void);
void time_square(void);
float square(float x);

//***************** Global Variables *********************//
char led_colour = 'r';
int led_on = 0; //0 = off, 1 = on

int sw1_ready = 1; // 0 = not ready, 1 = ready to accept next button press
int sw1_pressed = 0; // 0 = not pressed, 1 = pressed. sw1 pressed in the last 5 ms
int sw2_pressed = 0; // 0 = not pressed, 1 = pressed. sw2 pressed in the last 5 ms

int led_blinking = 1; //0 means led is not blinking, 1 means led is blinking

int SSD_counting = 1; //0 means paused, 1 means counting
int SSD_counter = 0;
//***************** Main *********************//

int main(void) {
	/*** Code here runs only once ***/

	//time_square();

	init_gpio();
	init_interrupts();

	led_on = 1;
	set_led(led_colour);
	write_7_segment_display("0000");




	/*** Code here repeats forever ***/
	while (1) {
	}
}

//***************** Function Definitions *********************//

void init_gpio(void) {

	volatile unsigned long delay_clk;
	SYSCTL_RCGC2_R |= 0x00000023; // Enable Clock Gating for Port A, B, and F, p.340
	SYSCTL_RCGCTIMER_R |= 0xF; // Enable Clock Gating for Timers 0, 1, 2, and 3, p.338
	delay_clk = SYSCTL_RCGC2_R;	  // Dummy operation to wait a few clock cycles
								  // See p.227 for more information.

	GPIO_PORTA_DEN_R |= 0xF0; //set PA[7:4] (LE[3:0] of display) to digital
	GPIO_PORTA_DIR_R |= 0xF0; //set PA[7:4] (LE[3:0] of display) direction to output
	GPIO_PORTA_AFSEL_R &= ~0xF0; //disable PB[7:0] (DATA[7:0] of display) alternate functions

	GPIO_PORTB_DEN_R |= 0xFF; //set PB[7:0] (DATA[7:0] of display) to digital
	GPIO_PORTB_DIR_R |= 0xFF; //set PB[7:0] (DATA[7:0] of display) direction to output
	GPIO_PORTB_AFSEL_R &= ~0xFF; //disable PB[7:0] (DATA[7:0] of display) alternate functions
	GPIO_PORTB_PUR_R |= 0xFF;


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

	//3. Write 0x2 into TnMR field of TIMER0_TAMR_R for periodic timer p.732
	// Write 0x1 into TACDIR field of TIMER0_TAMR_R for counting up p.731
	TIMER0_TAMR_R |= 0x12;

	//4. Load start value into TIMER0_TAILR_R
	TIMER0_TAILR_R = LED_TIMER_VALUE;

	//5. Disable interrupts using TIMER0_IMR_R
	TIMER0_IMR_R = 0x0;

	//6. Disable prescale using TIMER0_TAPR_R
	TIMER0_TAPR_R = 0x0;

	//7. Enable the timer
	TIMER0_CTL_R |= 0x1;


	/* Timer 1 initialization. Page 722 */
	TIMER1_CTL_R &= ~0x0;
	TIMER1_CFG_R  = 0x0;
	TIMER1_TAMR_R |= 0x11; //0x1 in bit field 1:0 for one shot mode and 0x1 in bit field 4 to count up. 0001 0001
	TIMER1_TAILR_R = SW_TIMER_VALUE;
	TIMER1_IMR_R = 0x0;
	TIMER1_TAPR_R = 0x0;

	/* Timer 2 initialization. Page 722 */
	TIMER2_CTL_R &= ~0x0;
	TIMER2_CFG_R  = 0x0;
	TIMER2_TAMR_R |= 0x11; //0x1 in bit field 1:0 for one shot mode and 0x1 in bit field 4 to count up. 0001 0001
	TIMER2_TAILR_R = SW_TIMER_VALUE;
	TIMER2_IMR_R = 0x0;
	TIMER2_TAPR_R = 0x0;

	/* Timer 3 initialization. Page 722 */
	TIMER3_CTL_R &= ~0x0; // p. 737 control
	TIMER3_CFG_R = 0x0; // p. 727 configuration
	TIMER3_TAMR_R |= 0x12; // preiodic timer counting up p. 729 timer mode.
	TIMER3_TAILR_R = SSD_TIMER_VALUE; // p. 756 interval load
	TIMER3_IMR_R = 0x0; // p. 745 interrupt mask
	TIMER3_TAPR_R = 0x0; // p. 760 prescale
	TIMER3_CTL_R |= 0x1;


	/*** SW2 Initialization. SW0 is connected to PF0 ***/
	//1. Unlock PF0 by writing 0x4C4F434B to GPIO_PORTF_LOCK_R (p.684)
	GPIO_PORTF_LOCK_R = 0x4C4F434B;

	//2. Enable writes to PF0 registers by writing 1 to bits 7:0 of GPIO_PORTF_CR_R (p.685)
	GPIO_PORTF_CR_R |= 0x0F;

	//3. Configure PF0 same as PF4
	GPIO_PORTF_DEN_R |= 0x01; //set PF0 (SW2) to digital
	GPIO_PORTF_DIR_R &= ~0x01; //set PF0 (SW2) direction to input
	GPIO_PORTF_AFSEL_R &= ~0x01; //disable PF0 (SW2) alternate functions
	GPIO_PORTF_PUR_R |= 0x01; //set the pull up resistor on PF0 (SW2)
}

void init_interrupts(void) {

	/* Timer 0A: Vector number = 35, interrupt number = 19, vector address = 0x0000 008C
		To enable interrupts for timer 0A, bit 19 of EN0 needs to be set
	*/
//	NVIC_EN0_R |= 0x1 << 19; // p.142
	TIMER0_IMR_R |= 0x1; //Enable time out interrupt mask p.747
	TIMER0_ICR_R |= 0x1; //write 1 to the TATOCINT to clear the timer interrupt p.755

	/* Timer 1A: Vector number = 37, interrupt number = 21 */
	NVIC_EN0_R |= 0x1 << 21; // p.142
	TIMER1_IMR_R |= 0x1; //Enable time out interrupt mask p.747
	TIMER1_ICR_R |= 0x1; //write 1 to the TATOCINT to clear the timer interrupt p.755

	/* Timer 2A: Vector number = 39, interrupt number = 23 */
	NVIC_EN0_R |= 0x1 << 23; // p.142
	TIMER2_IMR_R |= 0x1; //Enable time out interrupt mask p.747
	TIMER2_ICR_R |= 0x1; //write 1 to the TATOCINT to clear the timer interrupt p.755

	/* Timer 3A: Vector number = 51, interrupt number = 35 */
	NVIC_EN1_R |= 0x1 << (35-32); // p.142
	TIMER3_IMR_R |= 0x1; //Enable time out interrupt mask p.747
	TIMER3_ICR_R |= 0x1; //write 1 to the TATOCINT to clear the timer interrupt p.755

	/*
	GPIO Port F: Vector number = 46, interrupt number = 30, vector address = 0x0000 00B8
	*/
	NVIC_EN0_R |= 0x1 << 30; // p.142

	//Configure interrupts for switch 2 on PF0
	GPIO_PORTF_IS_R  &= ~0x1; // Set to 0 to detect edge p.664
	GPIO_PORTF_IEV_R &=  ~0x1; // Set to 0 to detect falling edge p.666
	GPIO_PORTF_IM_R  |=  0x1; // Set to 1 to send interrupts to interrupt controller p.667
	GPIO_PORTF_ICR_R  |= 0x1; // Set to 1 to clear interrupts p.670

	//Configure interrupts for switch 1 on PF4
	GPIO_PORTF_IS_R  &= ~(0x1 << 4);
	GPIO_PORTF_IEV_R |=  0x0 << 4;
	GPIO_PORTF_IM_R  |=  0x1 << 4;
	GPIO_PORTF_ICR_R  |= 0x1 << 4;

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

/* ISR for timer 0. Flashes the LED */
void timer0_handler(void) {
	if (led_on == 0) {
		led_on = 1;
		set_led(led_colour);
	}
	else {
		led_on = 0;
		set_led('o');
	}

	TIMER0_ICR_R |= 0x1; //writing a 1 to the TATOCINT to clear the timer interrupt

}

int port_f_nesting = 0;
/* ISR for Port F*/
void port_f_handler(void) {
	/*	SW1 and SW2 are connected on port F. Only one IRQ is generated
		per port so this handler must detect which switch was pressed
		and call the handlers for that swtich
	*/
	port_f_nesting++;
	if (port_f_nesting > 1)
		debug_printf("port_f_handler nested. port_f_nesting = %d\n", port_f_nesting);

	//SW1 connected on PF4 and SW2 on PF0
	int sw1_pressed = (GPIO_PORTF_MIS_R >> 4) & 0x1;
	int sw2_pressed = (GPIO_PORTF_MIS_R >> 0) & 0x1;

	if (sw1_pressed) {
		debug_printf("1. sw1 pressed\n");
		if (sw1_ready){
			debug_printf("2. sw1 ready\n");
			GPIO_PORTF_IM_R &= ~(0x1 << 4); //Disable interrupts for PF4 p.667
			change_led_colour();
			GPIO_PORTF_IM_R |= 0x1 << 4; //enable interrupts for PF4 p.667
		}

		//reset the timer back to 0 and mark the switch as not ready
		GPIO_PORTF_ICR_R |= 0x1 << 4; // Clear interrupts p.670
		TIMER1_ICR_R |= 0x1; // Clear the timer interrupt
		TIMER1_TAV_R = 0;
		TIMER1_CTL_R |= 0x1; // Enable timer 1
		sw1_ready = 0; //debouncing sw1 so it isnt ready to accept new presses
		debug_printf("3. sw1 timer reset\n");
	}

	if (sw2_pressed) {
		GPIO_PORTF_ICR_R |= 0x1;
		GPIO_PORTF_IM_R &= ~0x1;
		TIMER2_ICR_R |= 0x1; // Clear the timer interrupt
		TIMER2_CTL_R |= 0x1;

	}

	port_f_nesting--;
}

/* ISR for timer 1. Enables interrupts for SW1. Used for debouncing */
void timer1_handler(void) {
	static int prev_sw1_state = 1; //0 = press, 1 = not pressed
	/*SW1 is ready to accept the next button press if sw1 has been high
	for some time. When its bouncing, port_f_handler() will reset this timer
	so it won't time out and this handler will not be called. When this function is
	called, the button will have either just been pressed or just released. Both actions
	will cause bouncing. sw1_ready needs to be set only after the bouncing caused by
	releasing the button.
	*/

	//if timer times out but button is still pressed, then check back later
	int sw1_state = (GPIO_PORTF_DATA_R >> 4) & 0x01;

	debug_printf("4. timer1 sw1_state = %d\n", sw1_state);
	if (sw1_state == 0){
		TIMER1_TAV_R = 0;
		TIMER1_CTL_R |= 0x1; // Enable timer 1
		debug_printf("5. sw1 still down so check back later\n");
		return;
	}

	/*if timer times out and button is not pressed, then check back after
	some time. If the button is still not pressed after some time, then
	the button has been released and is not bouncing. In this case,
	sw1 is ready to accept new presses
	*/
	if (sw1_state == 1 && prev_sw1_state == 1) {
		debug_printf("6. sw1 remains unpressed after some time. sw1 ready\n");
		sw1_ready = 1;
		TIMER1_ICR_R |= 0x1; // Clear the timer interrupt
		return;
	}

	debug_printf("7. sw1 released for first time. Check back later\n");
	prev_sw1_state = sw1_state;
	TIMER1_ICR_R |= 0x1; // Clear the timer interrupt
	TIMER1_CTL_R |= 0x1; // Enable timer 1

}

/* ISR for timer 2. Enables interrupts for SW2. Used for debouncing */
void timer2_handler(void) {
	GPIO_PORTF_ICR_R |= 0x1;
	GPIO_PORTF_IM_R  |= 0x1;
	TIMER2_ICR_R |= 0x1;
	if(SSD_counting == 0)
	SSD_counting = 1;
	else
		SSD_counting = 0;
}

/* ISR for timer 3. Used for 7-segment display counter */
void timer3_handler(void) {
	if (SSD_counting == 0) return;
	SSD_counter++;
	if (SSD_counter > 9999) {
		SSD_counter = 0;
	}
	char SSD_counter_str[5];
	snprintf(SSD_counter_str, 5, "%4d", SSD_counter);
	write_7_segment_display(SSD_counter_str);
	TIMER3_ICR_R |= 0x1;
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


float square ( float x ) {
	float p ;
	p = x * x ;
	return ( p ) ;
}

void time_square(void) {
	volatile unsigned long delay_clk;
	SYSCTL_RCGC2_R |= 0x00000023; // Enable Clock Gating for Port A, B, and F, p.340
	SYSCTL_RCGCTIMER_R |= 0x10; // Enable Clock Gating for Timer 4, p.338
	delay_clk = SYSCTL_RCGC2_R;	  // Dummy operation to wait a few clock cycles
								  // See p.227 for more information.
	TIMER4_CTL_R &= ~0x0;
	TIMER4_CFG_R  = 0x0;
	TIMER4_TAMR_R |= 0x12;
	TIMER4_TAILR_R = 4294967295;
	TIMER4_IMR_R = 0x0;
	TIMER4_TAPR_R = 0x0;
	TIMER4_CTL_R |= 0x1;

	int num = 123456789;
	int start_time = TIMER4_TAV_R;
	int s = square(num);
	int end_time = TIMER4_TAV_R;

	int call_time = end_time - start_time;
	debug_printf("square(%d) takes %d clock cycles to run\n", num, call_time);
}
