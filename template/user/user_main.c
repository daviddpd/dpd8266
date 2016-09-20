#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "uart.h"
#include "espconn.h"
#include "mem.h"
#include "__attribute__.h"
#include "util.h"

int _GLOBAL_DEBUG;

static void helloWorld();

int i=0;
LOCAL os_timer_t poketimer;

static void CP_ICACHE_FLASH_ATTR
helloWorld()
{

	i++;
	CHATFABRIC_DEBUG_FMT(1, "Hellow World! %8d", i);  // macro contains new line;
		
}

//Init function 
void CP_ICACHE_FLASH_ATTR
user_init()
{

    uart_init(BIT_RATE_115200,BIT_RATE_115200);
	#ifdef VERSION_DATE
	#ifdef VERSION_GIT
	os_printf("0000000000 0000000000 0000000000 0000000000  %040x\n", 0);
	os_printf("%040x\n", 0);	
	os_printf("comnplie date: %s git:%s SDK Version: %s\n" , VERSION_DATE, VERSION_GIT, system_get_sdk_version() );
	CHATFABRIC_PRINT("\n");
	#endif
	#endif


	espWiFiInit();

    os_timer_disarm(&poketimer);
    os_timer_setfn(&poketimer, (os_timer_func_t *)helloWorld, NULL);
    os_timer_arm(&poketimer, 1000, 1);
}
