/*
Wahhaj Javed
muj975
*/

/*
This file contains definitions that map a character
to its data pins on a 7-segment display
*/

/*
0 = on, 1 = off
GPIODATA[7:0] = [DP, e, d, c, b, a, f, g]
                                            On segments
0               [ 1, 0, 0, 0, 0, 0, 0, 1]   abcdef
1               [ 1, 1, 1, 0, 0, 1, 1, 1]   bc
2               [ 1, 0, 0, 1, 0, 0, 1, 0]   abged
3               [ 1, 1, 0, 0, 0, 0, 1, 0]   abgcd
4               [ 1, 1, 1, 0, 0, 1, 0, 0]   fgbc
5               [ 1, 1, 0, 0, 1, 0, 0, 0]   afgcd
6               [ 1, 0, 0, 0, 1, 1, 0, 0]   fedcg
7               [ 1, 1, 1, 0, 0, 0, 1, 1]   abc
8               [ 1, 0, 0, 0, 0, 0, 0, 0]   abcdefg
9               [ 1, 1, 1, 0, 0, 0, 0, 0]   afbgc
.               [ 0, 1, 1, 1, 1, 1, 1, 1]   DP
A               [ 1, 1, 1, 1, 1, 1, 1, 1]   efabcg
B               [ 1, 1, 1, 1, 1, 1, 1, 1]   fe
C               [ 1, 1, 1, 1, 1, 1, 1, 1]
D               [ 1, 1, 1, 1, 1, 1, 1, 1]
E               [ 1, 1, 1, 1, 1, 1, 1, 1]
F               [ 1, 1, 1, 1, 1, 1, 1, 1]
G               [ 1, 1, 1, 1, 1, 1, 1, 1]
H               [ 1, 1, 1, 1, 1, 1, 1, 1]
I               [ 1, 1, 1, 1, 1, 1, 1, 1]
J               [ 1, 1, 1, 1, 1, 1, 1, 1]
K               [ 1, 1, 1, 1, 1, 1, 1, 1]
L               [ 1, 1, 1, 1, 1, 1, 1, 1]
M               [ 1, 1, 1, 1, 1, 1, 1, 1]
N               [ 1, 1, 1, 1, 1, 1, 1, 1]
O               [ 1, 1, 1, 1, 1, 1, 1, 1]
P               [ 1, 1, 1, 1, 1, 1, 1, 1]
Q               [ 1, 1, 1, 1, 1, 1, 1, 1]
R               [ 1, 1, 1, 1, 1, 1, 1, 1]
S               [ 1, 1, 1, 1, 1, 1, 1, 1]
T               [ 1, 1, 1, 1, 1, 1, 1, 1]
U               [ 1, 1, 1, 1, 1, 1, 1, 1]
V               [ 1, 1, 1, 1, 1, 1, 1, 1]
W               [ 1, 1, 1, 1, 1, 1, 1, 1]
X               [ 1, 1, 1, 1, 1, 1, 1, 1]
Y               [ 1, 1, 1, 1, 1, 1, 1, 1]
Z               [ 1, 1, 1, 1, 1, 1, 1, 1]




0               10000001    0x81
1               11100111    0xE7
2               10010010    0x92
3               11000010    0xC2
4               11100100    0xE4
5               11001000    0xC8
6               10001100    0x8C
7               11100011    0xE3
8               10000000    0x80
9               11100000    0xE0
.               01111111    0x7F
*/

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

