/*****************************************************
* Lite - this is the PWM input for the backlight control. Connect to 3-5VDC to turn on the backlight. Connect to ground to turn it off. Or, you can PWM at any frequency.
* MISO - this is the SPI Microcontroller In Serial Out pin, its used for the SD card. It isn't used for the TFT display which is write-only
* SCLK - this is the SPI clock input pin
* MOSI - this is the SPI Microcontroller Out Serial In pin, it is used to send data from the microcontroller to the SD card and/or TFT
* TFT_CS - this is the TFT SPI chip select pin
* Card CS - this is the SD card chip select, used if you want to read from the SD card.
* D/C - this is the TFT SPI data or command selector pin
* RST - this is the TFT reset pin. Connect to ground to reset the TFT! Its best to have this pin controlled by the library so the display is reset cleanly, but you can also connect it to the Arduino Reset pin, which works for most cases.
* Vcc - this is the power pin, connect to 3-5VDC - it has reverse polarity protection but try to wire it right!
* GND - this is the power and signal ground pin

TFT LCD (Pin)		LAUNCHPAD		(Pin)
---------------------------------------------------
LITE 	(pin 10) 	+3.3 V			J1-1
MISO 	(pin 9) 	NOT USED
SCK 	(pin 8) 	PA2 (SSI0Clk)	J2-10
MOSI 	(pin 7) 	PA5 (SSI0Tx)	J1-8
TFT_CS 	(pin 6) 	PA3 (SSI0Fss)	J2-9
CARD_CS	(pin 5) 	NOT USED
D/C 	(pin 4) 	PA6 (GPIO)		J1-9
RST 	(pin 3) 	PA7 (GPIO)		J1-10
VCC 	(pin 2) 	+3.3 V			J1-1
GND 	(pin 1) 	GND				J3-2

*******************************************************/


/********* From Tables 2-3, 2-4 in user manual
 *
 * SSI PIN		GPIO PIN	GPIOCTL
 *
 * SSI0CLK		PA2			2
 * SSI0Fss		PA3			2
 * SSI0RX		PA4			2
 * SSI0TX		PA5			2
 *
**********/

/* Clock Gating Control */
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_RCGCSSI_R (*((volatile unsigned long *)0x400FE61C))

/* GPIO PINS */
#define GPIOA_BASE_R	(*((volatile unsigned *) 0x40004000))
#define GPIODATA_A_R	(*((volatile unsigned *) (GPIOA_BASE_R + 0x3FC)))
#define GPIODIR_A_R		(*((volatile unsigned *) (GPIOA_BASE_R + 0x400)))
#define GPIOAFSEL_A_R	(*((volatile unsigned *) (GPIOA_BASE_R + 0x420)))
#define GPIODEN_A_R		(*((volatile unsigned *) (GPIOA_BASE_R + 0x51C)))
#define GPIOPCTL_A_R	(*((volatile unsigned *) (GPIOA_BASE_R + 0x52C)))


void init_gpio(void) {
	volatile unsigned long delay_clk;
	SYSCTL_RCGCGPIO_R |= 0x01; // Enable Clock Gating for PortA, p.340
	delay_clk = SYSCTL_RCGCGPIO_R;	  // Dummy operation to wait a few clock cycles
								  // See p.227 for more information.

	/* Configuration for D/C on PA6*/
	GPIODIR_A_R &= ~0x40; //set PA6 to input p.663
	GPIOAFSEL_A_R &= ~0x40; //disable PA6 alternative functions p.670
	GPIODEN_A_R |= 0x40; //Set PA6 to digital

	/* Configuration for RST on PA7*/
	GPIODIR_A_R &= ~0x80; //set PA7 to input p.663
	GPIOAFSEL_A_R &= ~0x80; //disable PA7 alternative functions p.670
	GPIODEN_A_R |= 0x80; //Set PA7 to digital
}


void init_ssi0(void) {
	/* SSI configuration on p.965*/
	//1. Enable the SSI and GPIO modules using the RCGCSSI and RCGCGPIO registers
	volatile unsigned long delay_clk;
	SYSCTL_RCGCSSI_R |= 0x1; //Enable Clock gating for SSI0 p.346
	SYSCTL_RCGCGPIO_R |= 0x01; // Enable Clock Gating for PortA, p.340
	volatile unsigned long delay_clk = SYSCTL_RCGCSSI_R;
	delay_clk = SYSCTL_RCGCGPIO_R;	  // Dummy operation to wait a few clock cycles


	//2. Configure port A GPIO

	//SSI0CLK on PA2
	GPIODIR_A_R &= ~0x02; //set PA2 to input p.663
	GPIODEN_A_R |= 0x02; //Set PA2 to digital
	GPIOAFSEL_A_R &= ~0x02; //disable PA2 alternative functions p.670

	//SSI0Fss on PA3
	GPIODIR_A_R &= ~0x04;
	GPIODEN_A_R |= 0x04;
	GPIOAFSEL_A_R &= ~0x04;

	//SSI0TX on PA5
	GPIODIR_A_R &= ~0x20;
	GPIODEN_A_R |= 0x20;
	GPIOAFSEL_A_R |= 0x20; // Enable alternative functions on PA5 p.671
	GPIOPCTL_A_R |= 0x2 << 2; //Set PA[5:2] to 0x2 for SSI0 p.688

	/* Configuration for D/C on PA6*/
	GPIODIR_A_R &= ~0x40;
	GPIODEN_A_R |= 0x40;
	GPIOAFSEL_A_R &= ~0x40;

	/* Configuration for RST on PA7*/
	GPIODIR_A_R &= ~0x80;
	GPIODEN_A_R |= 0x80;
	GPIOAFSEL_A_R &= ~0x80;


	//5. Program the GPIODEN register to enable the pin's digital function. In addition, the drive strength,


}












/*** Definition of BASE registers for SSI0 ***/
#define SSI0_CR0_R              (*((volatile unsigned long *)0x40008000)) // refer pg 937. Base address of SSI0 is 0x40008000 Control0 register offset is 0x000, Necessary for using SSI0
#define SSI0_CR1_R              (*((volatile unsigned long *)0x40008004)) // refer pg 937. Base address of SSI0 is 0x40008000 Control1 register offset is 0x004, Necessary for using SSI0
#define SSI0_DR_R               (*((volatile unsigned long *)0x40008008)) // refer pg 937. Base address of SSI0 is 0x40008000 SSI data register offset is 0x008, Necessary for using SSI0
#define SSI0_SR_R               (*((volatile unsigned long *)0x4000800C)) // refer pg 937. Base address of SSI0 is 0x40008000 SSI status register offset is 0x00C, Necessary for using SSI0
#define SSI0_CPSR_R             (*((volatile unsigned long *)0x40008010)) // refer pg 937. Base address of SSI0 is 0x40008000 SSI clock prescale register offset is 0x010, Necessary for using SSI0
#define SSI0_CC_R               (*((volatile unsigned long *)0x40008FC8)) // refer pg 938/ 954. Base address of SSI0 is 0x40008000 Clock configuration offset is 0xFC8, Necessary for using SSI0

/*** Register Masking for SSI register setups ***/
#define SSI_CR0_SCR_M           0x0000FF00  // refer pg 939. SSI Serial Clock Rate
#define SSI_CR0_SPH             0x00000080  // refer pg 939. SSI Serial Clock Phase
#define SSI_CR0_SPO             0x00000040  // refer pg 939. SSI Serial Clock Polarity
#define SSI_CR0_FRF_M           0x00000030  // refer pg 939. SSI Frame Format Select

/*** Register Data masking for SSI setup  ****
* These Register maskings are specially coded for support of SPI to use with ST7735 TFT LCD
***********************************************/

#define SSI_CR0_FRF_MOTO        0x00000000  // refer pg 940.Freescale SPI Frame Format , FRF bits value is 0x0
#define SSI_CR0_DSS_M           0x0000000F  // refer pg 939. SSI Data Size Select Masking of last 4 bits
#define SSI_CR0_DSS_8           0x00000007  // refer pg 940. SSI Data Size Select ,8-bit data , 0x07
#define SSI_CR1_MS              0x00000004  // refer pg 941. SSI Master/Slave Select Masking of 4th bit
#define SSI_CR1_SSE             0x00000002  // refer pg 941. SSI Synchronous Serial Port Masking of 2nd bit

#define SSI_SR_BSY              0x00000010  // refer pg 944. SSI Busy Bit, 5th bit in SSI status register
#define SSI_SR_TNF              0x00000002  // refer pg 944. SSI Transmit FIFO Not Full, 2nd bit is SSI status register
#define SSI_CPSR_CPSDVSR_M      0x000000FF  // refer pg 946. SSI Clock Prescale Divisor, last 8 bits in SSI Clock Prescale register
#define SSI_CC_CS_M             0x0000000F  // refer pg 954. SSI Baud Clock Source, last 4 bits of clock configuration register
#define SSI_CC_CS_SYSPLL        0x00000000  // refer pg 954. SSI clock source is System clock ( based on clock source and divisor factor)

/*** Definition for BASE register for Module clocking  ****
* These Register maskings are specially coded for support of SPI and GPIOA
***********************************************/
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104)) // refer pg 450. This register will used to enable clock for SSI, I2C, TIMER, UART
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108)) // refer pg 453. This register will used to enable clock for GPIO
#define SYSCTL_RCGC1_SSI0       0x00000010  // refer pg 450. SSI0 Clock Gating Control, Set 4th Bit of RCGC1 to enable clock for SSI0
#define SYSCTL_RCGC2_GPIOA      0x00000001  // refer pg 453. port A Clock Gating Control, Set 1st Bit of RCGC2 to enable clock for GPIOA



/**** Initialization function for SSI0  Module ********/
void init_SSI0(void)
{
  GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2,PA3,PA5 binary 0010 1100
  GPIO_PORTA_DEN_R |= 0x2C;             // enable digital I/O on PA2,PA3,PA5 binary 0010 1100
										// configure PA2,PA3,PA5 as SSI
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;  // refer pg 670 Port MUX control for GPIO1 ,GPIO2 and GPIO 4 ( PA2, PA3, PA4)m writing 0x02 gives its SSI functionality
  GPIO_PORTA_AMSEL_R &= ~0x2C;          // disable analog functionality on PA2,PA3,PA5
  SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI (SSE=0)( we need to disable SSI before any control registers are reporgrammed!! pg, 942)
  SSI0_CR1_R &= ~SSI_CR1_MS;            // Enable master mode ( MS=0)
										// refer pg 954. configure for system clock/PLL baud clock source
  SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL; // Mask last 4 bits and fill it with zeros simply
										// refer pg 946. clock divider for 3.125 MHz SSIClk we have BR=SysClk/(CPSDVSR * (1 + SCR)), here BR=3.125Mbps, SysClk =50Mhz, SCR=0 so CPSDVSR=16
  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+16;
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (3.125 Mbps data rate) BR=SysClk/(CPSDVSR * (1 + SCR))
				  SSI_CR0_SPH |         // SPH = 0 Data is captured on the first clock edge transistion, (rising edge)
				  SSI_CR0_SPO);         // SPO = 0 Steady state Low value is placed on the SSIClk pin (default state of clk pin is LOW)
										// FRF = Freescale format
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
										// DSS = 8-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;
  SSI0_CR1_R |= SSI_CR1_SSE;            // Finally enable SSI after  configuration of all parameters

}

// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.
// NOTE: These functions will crash or stall indefinitely if
// the SSI0 module is not initialized and enabled.
void static writecommand(unsigned char c) {
										// wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  DC = DC_COMMAND;
  SSI0_DR_R = c;                        // Write data to SSI Data Register
										// wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
}


void static writedata(unsigned char c) {
  while((SSI0_SR_R&SSI_SR_TNF)==0){};   // wait until transmit FIFO not full
  DC = DC_DATA;
  SSI0_DR_R = c;                        // data out
}