#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG
	#include <stdio.h>
	#define debug_printf(...) printf(__VA_ARGS__)
#else
	#define debug_printf(...) ((void)0)
#endif

#endif // __DEBUG_H__
