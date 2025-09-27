/* CME 331 Assignment 3
Wahhaj Javed
muj975
*/


/**** Q1. Port A Definitions ****/

#define GPIO_PORTA_DATA_R (*((volatile unsigned long *)  ( 0x40058000 + 0x3FC )))	
#define GPIO_PORTA_DIR_R (*((volatile unsigned long *)   ( 0x40058000 + 0x400 )))	
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long *) ( 0x40058000 + 0x420 )))
#define GPIO_PORTA_DEN_R (*((volatile unsigned long *)   ( 0x40058000 + 0x51C )))	
#define GPIO_PORTA_PUR_R (*((volatile unsigned long *)   ( 0x40058000 + 0x510 )))	



/**** Q2. Timer 0 Definitions ****/
#define TIMER0_CFG_R (*((volatile unsigned long *)        0x40030000 ))          // p. 727 configuration
#define SYSCTL_RCGCTIMER_R (*((volatile unsigned long *) (0x400FE000 + 0x604) )) //p. 338
#define TIMER0_CTL_R (*((volatile unsigned long *)       (0x40030000 + 0x00C ))) // p. 737 control
#define TIMER0_TAMR_R (*((volatile unsigned long *)      (0x40030000 + 0x004 ))) // p. 729 prescale
#define TIMER0_IMR_R (*((volatile unsigned long *)       (0x40030000 + 0x018 ))) // p. 745 interrupt mask
#define TIMER0_TAILR_R (*((volatile unsigned long *)     (0x40030000 + 0x028 ))) // p. 756 interval load
#define TIMER0_TAPR_R (*((volatile unsigned long *)      (0x40030000 + 0x038 ))) // p. 760 
#define TIMER0_TAV_R (*((volatile unsigned long *)       (0x40030000 + 0x050 ))) // p. 766 Timer value
#define TIMER0_RIS_R (*((volatile unsigned long *)       (0x40030000 + 0x01C ))) // p. 748 Interrupt values
#define TIMER0_ICR_R (*((volatile unsigned long *)       (0x40030000 + 0x024 ))) // p. 754 Interrupt clear


#define TICKS_PER_SEC 16000000 //16E6 ticks per min


/* Q1 Register Configuration for port A */
void init_port_a(void) {
	SYSCTL_RCGC2_R |= 0x01; // Enable Clock Gating for PortA, p.340

    /* Using ports PA2, PA3, and PA4 */

	GPIO_PORTA_DEN_R |= 0x04; //set PA2 to digital
	GPIO_PORTA_DIR_R &= ~0x04; //set PA2 direction to input
	GPIO_PORTA_AFSEL_R &= ~0x04; //disable PA2 alternate functions

	GPIO_PORTA_DEN_R |= 0x08; //set PA3 to digital
	GPIO_PORTA_DIR_R &= ~0x08; //set PA3 direction to input
	GPIO_PORTA_AFSEL_R &= ~0x08; //disable PA3 alternate functions

	GPIO_PORTA_DEN_R |= 0x10; //set PA4 to digital
	GPIO_PORTA_DIR_R &= ~0x10; //set PA4 direction to input
	GPIO_PORTA_AFSEL_R &= ~0x10; //disable PA4 alternate functions


}

/* Q2 Register Configuration for timer 0 */
void init_timer_0(void) {
    SYSCTL_RCGCTIMER_R |= 0x01; // Enable Clock Gating for Timers 0 p.338

    /* Timer 0 initialization. Page 722 */
	//1. Clear TAEN bit in TIMER0_CTL_R
	TIMER0_CTL_R &= ~0x0;

	//2. Set TIMER0_CFG_R to 0x00000000
	TIMER0_CFG_R  = 0x0;

	//3. Write 0x2 into TnMR filed of TIMER0_TAMR_R
	TIMER0_TAMR_R |= 0x2;

	//4. Load start value into TIMER0_TAILR_R
	TIMER0_TAILR_R = TICKS_PER_SEC * 60 ;

	//5. Disable interrupts using TIMER0_IMR_R
	TIMER0_IMR_R = 0x0;

	//6. Disable prescale using TIMER0_TAPR_R
	TIMER0_TAPR_R = 0x0;

	//7. Enable the timer
	TIMER0_CTL_R |= 0x1;
}


/* Q3 */
int main(void) {
    init_port_a();
    init_timer_0();

    /* 
    PA2 connected to X, PA3 connected to Y, and PA4 connected to Z 
    PA2 and PA4 will normally be low. When the desk moves up, PA2 goes high
    When the desk moves down, PA4 goes high
    */
    

    //'u' for up, 'd' for down, 'n' for none. The direction in which the
    //desk was moved by the roomate. 'n' is the starting condition
    char adjustment_dir = 'n' 

    //How many minutes since the desk was adjusted by roomate
    int minutes_since_adjustment = -1;

    //The desk is ready to be moved after 5 minutes
    int five_minutes_elapsed = 0;


    while(1) {

        /*** 1. Detect desk movement ***/

        //PA2 high to desk moved up
        if( (GPIO_PORTA_DATA_R >> 2) & 0x01 ) {
            adjustment_dir = 'u';
            minutes_since_adjustment = 0;
            five_minutes_elapsed = 0;
        }
        //PA4 high to desk moved down
        else if( (GPIO_PORTA_DATA_R >> 4) & 0x01 ) {
            adjustment_dir = 'd';
            minutes_since_adjustment = 0;
            five_minutes_elapsed = 0;
        }


        /*** 2. Wait for 5 minutes. This code won't run until 
        minutes_since_adjustment is set to 0 by the previous code. 
        
        Timer value won't be read sequentially because of all the other
        code in the loop so TAV == 0 will rarely succeed. 
        ***/
        if(five_minutes_elapsed == 0 && TIMER0_TAV_R < 1000) {
            minutes_since_adjustment++;
            if (minutes_since_adjustment >= 5)
                five_minutes_elapsed = 1;
        }

        /*** 3. After 5 minutes, the microcontroller can start moving the desk.
        The movement can only happen in the last 1 second which happens
        when the timer value is less than 16000000. Timer will automatically
        restart when it reaches 0 so the lower bound doesn't need to be 
        checked here.

        The pins will need to be set to output mode so they can drive the motor.
        This means that the for 1 second of every minute, the microcontroller
        can't detect whether the desk is being adjusted by the roomate. But
        the roommate will likely press the switch for longer than 1
        second so it is unlikely for a switch press to be missed.
        ***/
        while (five_minutes_elapsed == 1 && TIMER0_TAV_R < TICKS_PER_SEC ) {
            GPIO_PORTA_DIR_R |= 0x04; //set PA2 direction to output
            GPIO_PORTA_DIR_R |= 0x10; //set PA4 direction to output
            if (adjustment_dir == 'u')
                //set PA4 (Z) to high so desk moves down
                GPIO_PORTA_DATA_R |= 0x10; 
            else if (adjustment_dir == 'd')
                //set PA2 (X) to high so desk moves up
                GPIO_PORTA_DATA_R |= 0x04; 
        }
        GPIO_PORTA_DIR_R &= ~0x04; //set PA2 direction to input
        GPIO_PORTA_DIR_R &= ~0x10; //set PA4 direction to input
    }
}