#ifndef CF_UTIL_H
#define CF_UTIL_H

#include "__attribute__.h"
#include <string.h>
#include <stdarg.h>
void CP_ICACHE_FLASH_ATTR util_bin2hex (char *cd, char* label, unsigned char * x, int len );
void CP_ICACHE_FLASH_ATTR util_debug_bin2hex(char* label, unsigned char * x, int len,  char* file, const char* func, int line );
#define util_print_bin2hex( a, b ) ((void)0)

extern int _GLOBAL_DEBUG;
//extern macaddr_t macAddr;

#ifdef ESP8266
#include "ets_sys.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
void* calloc_wrapper(size_t len, size_t size);


#define socklen_t int
#define malloc os_malloc
#define calloc(len,size) calloc_wrapper(len,size);
#define free os_free
#define arc4random_uniform(x) os_random() % x
#define printf(...) os_printf( __VA_ARGS__ )
#define sprintf(...) os_sprintf( __VA_ARGS__ )
#define sbuf_printf(...) os_sprintf( __VA_ARGS__ )

extern int os_printf_plus(const char * format, ...);


#define CHATFABRIC_PRINT(msg) os_printf("%s\n", msg );
#define CHATFABRIC_DEBUG(d, msg) if (d) os_printf("[DEBUG][%s:%s:%d] %s\n", __FILE__, __FUNCTION__, __LINE__, msg );
#define CHATFABRIC_DEBUG_FMT(d, fmt, ...) if (d) os_printf("[DEBUG][%s:%s:%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ );
#define CHATFABRIC_DEBUG_FMT2(d, func, line, fmt, ...) if (d) os_printf("[DEBUG][%s:%s:%d] " fmt "\n", __FILE__, func, line, __VA_ARGS__ );
#define CHATFABRIC_DEBUG_B2H(d, label, x, len) if (d) util_debug_bin2hex(label, x,len, __FILE__, __FUNCTION__, __LINE__ );

#else 

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define CHATFABRIC_PRINT(msg) printf("%s\n", msg );
#define CHATFABRIC_DEBUG(d, msg) if (d) printf("[DEBUG][%s:%s:%d] %s\n", __FILE__, __FUNCTION__, __LINE__, msg );
#define CHATFABRIC_DEBUG_FMT(d, fmt, ...) if (d) printf("[DEBUG][%s:%s:%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ );
#define CHATFABRIC_DEBUG_FMT2(d, func, line, fmt, ...) if (d) printf("[DEBUG][%s:%s:%d] " fmt "\n", __FILE__, func, line, __VA_ARGS__ );
#define CHATFABRIC_DEBUG_B2H(d, label, x, len) if (d) util_debug_bin2hex(label, x,len, __FILE__, __FUNCTION__, __LINE__ );

#endif


#endif
