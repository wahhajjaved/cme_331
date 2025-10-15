/**
 * main.c
 */

#include "lcd_extra_files/ST7735.h"

int main(void)
{

	ST7735_InitR(INITR_GREENTAB);
	unsigned short color = ST7735_Color565(0, 0, 0);

	while(1) {
		int counter = 1000000;
		while(--counter > 0){}
		ST7735_FillScreen(color++);
	}

	return 0;
}
