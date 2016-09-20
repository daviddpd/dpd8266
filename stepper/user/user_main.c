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
#include "gpio.h"


static void helloWorld();

int h=0;
int stepper=0;
LOCAL os_timer_t poketimer;

static void CP_ICACHE_FLASH_ATTR
helloWorld()
{
//	int gpio[4] = {4,5,12,13};
	int gpio[4] = {13, 13, 5, 4};
	int i,x;

	h++;
	CHATFABRIC_DEBUG_FMT(1, "Hellow World! %8d", h);  // macro contains new line;

	system_soft_wdt_stop();
	
	for (x=0; x<100; x++) {
		for (i=0; i<4; i++) {
			GPIO_OUTPUT_SET(gpio[i], stepper);
			os_delay_us(2000);
		}
		stepper = !stepper;    
	}

	system_soft_wdt_restart();
	
    os_timer_arm(&poketimer, 5, 0);
		
}

//Init function 
void CP_ICACHE_FLASH_ATTR
user_init()
{

    uart_init(BIT_RATE_115200,BIT_RATE_115200);

    gpio_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);

    os_timer_disarm(&poketimer);
    os_timer_setfn(&poketimer, (os_timer_func_t *)helloWorld, NULL);
    os_timer_arm(&poketimer, 250, 0);
    
        
    
}
