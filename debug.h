//#ifndef _DEBUG_H
//#define _DEBUG_H

//#define NDEBUG

#ifdef NDEBUG
	#define debug_print(fmt, ...) ((void)0)
	#define debug_puts(msg) ((void)0)
#else
	#define debug_print(fmt, ...) \
	fprintf(stderr, "%s() in %s, line %i: " fmt "\n", \
			__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
	#define debug_puts(msg) debug_print("%s", msg)
#endif
