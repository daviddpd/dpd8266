#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "uart.h"
#include "espconn.h"
#include "mem.h"
#include "__attribute__.h"
#include "ntp.h"

int bootstatus = 0;  // 1 - network up, 2-ready
uint32_t t = 0;
uint32_t  heapLast = 0;
uint32_t  heap = 0;
uint8_t ESP_WORD_ALIGN wifiStatus;
int ntp_unix_timestamp = 0;
int ntp_status = -1;

static void loop();

LOCAL os_timer_t boottimer;
LOCAL os_timer_t poketimer;

typedef struct {
	int socket;
#ifdef ESP8266
	struct espconn tcpconn;
	struct espconn udpconn;
	struct espconn *conn;
	esp_tcp esptcp;
#else
	struct sockaddr_in sockaddr;
#endif
	char *ip;
	int type; // SOCK_STREAM SOCK_DGRAM SOCK_RAW SOCK_RDM SOCK_SEQPACKET
	int acceptedSocket;
	int bind;
} ESP_WORD_ALIGN _Connection;


_Connection ESP_WORD_ALIGN c;

LOCAL void CP_ICACHE_FLASH_ATTR
udp_callback(void *arg, char *data, unsigned short length)
{
	unsigned short l;
    os_printf("%12u GOT A UDP PACKET\n\r", t/100000 );

    if (data == NULL) { return; }
    
	return;
}

tcp_listen(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_recvcb(pesp_conn, udp_callback);
    
//    espconn_regist_reconcb(pesp_conn, webserver_recon);
//    espconn_regist_disconcb(pesp_conn, webserver_discon);
}

static void CP_ICACHE_FLASH_ATTR
loop()
{
	t = system_get_time();
	heapLast = heap;
	heap = system_get_free_heap_size();
	if (ntp_unix_timestamp > 0) {
	    ntp_unix_timestamp++;
	}
	
	wifiStatus = wifi_station_get_connect_status();
//    os_printf("%12u wifi connect status -  %d/%d\n\r", t/100000, wifiStatus, bootstatus);
    os_printf("%12u %12u Running -  %d/%d\n\r", t/100000, ntp_unix_timestamp, wifiStatus, bootstatus);
	
}

static void CP_ICACHE_FLASH_ATTR
startup()
{
	uint32 t;
	uint32_t status;
	char buffer[128] = {0};
    struct ip_info ipconfig;
    char hwaddr[6];
	t = system_get_time();
	wifiStatus = wifi_station_get_connect_status();
    os_printf("%12u %12u wifi connect status -  %d/%d\n\r", t/100000, ntp_unix_timestamp, wifiStatus, bootstatus);

	if ( ( wifiStatus == STATION_GOT_IP ) &&  ( bootstatus == 0 ) ) {
		os_timer_disarm(&boottimer);
        wifi_get_ip_info(STATION_IF, &ipconfig);
        wifi_get_macaddr(STATION_IF, hwaddr);
        os_sprintf(buffer, MACSTR " " IPSTR, 
                   MAC2STR(hwaddr), IP2STR(&ipconfig.ip));
		os_printf("%s\n\r", buffer);

		bootstatus = 1; // network up
#ifdef BROKEN		
		ntp_get_time();	
#else 
		ntp_unix_timestamp = 1439183355;
#endif 
		os_timer_disarm(&boottimer);
	    os_timer_setfn(&boottimer, (os_timer_func_t *)startup, NULL);
    	os_timer_arm(&boottimer, 250, 1);
		
	} 

	if ( (ntp_status == 2) && ( bootstatus == 1 ) ) {
		os_printf ("ntp timed out ... retry.\n");
		ntp_get_time();	
	}
	
	if ( (ntp_unix_timestamp > 0) && ( bootstatus == 1 ) ) {
		os_timer_disarm(&boottimer);

	    os_timer_disarm(&poketimer);
    	os_timer_setfn(&poketimer, (os_timer_func_t *)loop, NULL);
    	os_timer_arm(&poketimer, 1000, 1);

		bootstatus = 2; // timeset 
	    os_printf("%12u %12u Boot Status -  %d/%d\n\r", t/100000, ntp_unix_timestamp, wifiStatus, bootstatus);

        wifi_get_ip_info(STATION_IF, &ipconfig);
		struct mdns_info *info = (struct mdns_info *)os_zalloc(sizeof(struct mdns_info));
		info->ipAddr = ipconfig.ip.addr; //ESP8266 station IP
		
		info->server_name = "http";
		info->host_name = "HostName8266";

        wifi_get_macaddr(STATION_IF, hwaddr);
        wifi_get_ip_info(STATION_IF, &ipconfig);
		
		info->server_port = 1288;
		info->txt_data[0] = "version = 0.1";
		info->txt_data[1] = "user1 = data1";
		info->txt_data[2] = "user2 = data2";
		espconn_mdns_init(info);


		c.tcpconn.type = ESPCONN_TCP;
		c.tcpconn.state = ESPCONN_NONE;
		c.tcpconn.proto.tcp = &c.esptcp;
		c.tcpconn.proto.tcp->local_port = 1288;
		espconn_regist_connectcb(&c.tcpconn, (espconn_connect_callback)tcp_listen);
		espconn_accept(&c.tcpconn);
	    
    	c.udpconn.type = ESPCONN_UDP;
    	c.udpconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    	c.udpconn.proto.udp->local_port = 1288;
    	espconn_regist_recvcb(&c.udpconn, udp_callback);
    	espconn_create(&c.udpconn);

	    os_printf("%12u %12u Setup Complete -  %d/%d\n\r", t/100000, ntp_unix_timestamp, wifiStatus, bootstatus);


	} 
	
}


//Init function 
void CP_ICACHE_FLASH_ATTR
user_init()
{
    char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;
    int i=0;

    uart_init(BIT_RATE_115200,BIT_RATE_115200);

	heap =0;
	heapLast = 0;
    //Set station mode
    wifi_set_opmode( 0x1 );

    //Set ap settings
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64);
    wifi_station_set_config(&stationConf);

    os_timer_disarm(&boottimer);
    os_timer_setfn(&boottimer, (os_timer_func_t *)startup, NULL);
    os_timer_arm(&boottimer, 250, 1);
}
