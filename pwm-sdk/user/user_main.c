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
#include "pwm.h"

#define PWM_12_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_12_OUT_IO_NUM 12
#define PWM_12_OUT_IO_FUNC  FUNC_GPIO12

#define PWM_14_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_14_OUT_IO_NUM 14
#define PWM_14_OUT_IO_FUNC  FUNC_GPIO14

int _GLOBAL_DEBUG = 1;

static void helloWorld();

int i=0;
int direction = 0;
LOCAL os_timer_t poketimer;

static void CP_ICACHE_FLASH_ATTR
helloWorld()
{

	uint32 duty = 0;
	uint32 period = 0;
	uint32 period_max =0;;
	uint32 perunit =0;
	
	double period_percent;

	period = pwm_get_period();
	period_max = ((period * 1000) / 45);
	
	period_percent = ((i % 100) / 100);
	
	perunit = period_max/100;
	
	duty = (i % 100) * perunit;
	if ( direction == 1) {
		duty =  100*perunit - duty;
	}
	CHATFABRIC_DEBUG_FMT( _GLOBAL_DEBUG, "=== Control Dimmer: Duty: %d  Period: %d Period Max: %d %d %d %d %d", duty, period, period_max, (int)(period_percent*100), (i % 100), perunit, direction);
	
	pwm_set_duty( duty, 0 ); 
	pwm_start();
	if ( (i % 100) == 99 ) {
		direction = !direction;
	}

	i++;
		
}


void pwm_setup() 
{
	int i=0, x=0;
	uint32 channel = 0;
	uint32 duty_percent;
	uint32 duty = 0;
	unsigned char *tmp;
    uint32 pwm_duty_init[1] = {0};
    
	uint32 io_info[][3] = { 
		{PWM_14_OUT_IO_MUX,PWM_14_OUT_IO_FUNC,PWM_14_OUT_IO_NUM}		
	};
	
    set_pwm_debug_en(0);//disable debug print in pwm driver
    pwm_init(1000,  pwm_duty_init ,1, io_info);
	pwm_start();


}


//Init function 
void CP_ICACHE_FLASH_ATTR
user_init()
{

    uart_init(BIT_RATE_115200,BIT_RATE_115200);
	#ifdef VERSION_DATE
	#ifdef VERSION_GIT
	os_printf("0000000000 0000000000 0000000000 0000000000  %040x\n", 0);
	os_printf("0000000000 0000000000 0000000000 0000000000  %040x\n", 0);
	os_printf("comnplie date: %s git:%s SDK Version: %s\n" , VERSION_DATE, VERSION_GIT, system_get_sdk_version() );
	CHATFABRIC_PRINT("\n");
	#endif
	#endif


	espWiFiInit();
	pwm_setup();
	
    os_timer_disarm(&poketimer);
    os_timer_setfn(&poketimer, (os_timer_func_t *)helloWorld, NULL);
    os_timer_arm(&poketimer, 100, 1);
}
