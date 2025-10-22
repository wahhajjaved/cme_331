#ifndef __HELPERS_H__
#define __HELPERS_H__


#define CLOCK_FREQUENCY_MS 16000 //16E6 ticks per second to 16000 ticks per ms


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


/*
* state = 'o' --> off
* state = 'r' --> red led on
* state = 'g' --> green led on
* state = 'b' --> blue led on
* state = 'w' --> white led on
*/
void set_led(char state);

/*
Write the given string to the 4 7-segment LED displays.
output_string must be a null terminated string of size 4 or less
not counting the null terminator. Currently only supports
characters '0' - '9', '.', and ' '
*/
void write_7_segment_display(char *output_string);
int char_to_display_data(char c);

#endif //__HELPERS_H__
