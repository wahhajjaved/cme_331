#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
	#include <stdio.h>
	#define debug_printf(...) printf(__VA_ARGS__)
#else
	#define debug_printf(...) ((void)0)
#endif

#endif // DEBUG_H
