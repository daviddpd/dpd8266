#include "esp-cf-wifi.h"
#include "util.h"

extern int _GLOBAL_DEBUG;

struct station_config stationConf;

//void CP_ICACHE_FLASH_ATTR changeMode(enum deviceModes m);



void CP_ICACHE_FLASH_ATTR
espCfWiFi_callBack(System_Event_t *evt)
{

	// CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "event %02x", evt->event);
	switch (evt->event) {
		case EVENT_STAMODE_CONNECTED:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "connect to ssid %s, channel %d",
			evt->event_info.connected.ssid,
			evt->event_info.connected.channel);			
		break;
		case EVENT_STAMODE_DISCONNECTED:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "disconnect from ssid %s, reason %d",
			evt->event_info.disconnected.ssid,
			evt->event_info.disconnected.reason);
		break;
		case EVENT_STAMODE_AUTHMODE_CHANGE:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "mode: %d -> %d",
			evt->event_info.auth_change.old_mode,
			evt->event_info.auth_change.new_mode);

		break;
		case EVENT_STAMODE_GOT_IP:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR " ",
			IP2STR(&evt->event_info.got_ip.ip),
			IP2STR(&evt->event_info.got_ip.mask),
			IP2STR(&evt->event_info.got_ip.gw));
			
		break;
		case EVENT_SOFTAPMODE_STACONNECTED:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG, "station: " MACSTR "join, AID = %d",
			MAC2STR(evt->event_info.sta_connected.mac),
			evt->event_info.sta_connected.aid);

		break;
		case EVENT_SOFTAPMODE_STADISCONNECTED:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "station: " MACSTR "leave, AID = %d",
			MAC2STR(evt->event_info.sta_disconnected.mac),
			evt->event_info.sta_disconnected.aid);
		break;
/*		case EVENT_SOFTAPMODE_PROBEREQRECVED:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  
				"SOFTAPMODE_PROBEREQRECVED:  %02x ; RSSI %d ; " MACSTR " ", 
				evt->event, 
				evt->event_info.ap_probereqrecved.rssi,
				MAC2STR(evt->event_info.ap_probereqrecved.mac) 
			);
		break;
*/
		default:
			CHATFABRIC_DEBUG_FMT(_GLOBAL_DEBUG,  "UNKNOWN EVENT:  %02x", evt->event );
		break;
	}
}


void CP_ICACHE_FLASH_ATTR
espWiFiInit()
{

/*
	SOFTAP_MODE;
	STATIONAP_MODE;
	SOFTAP_MODE;
*/
  
int mode = STATION_MODE;
  
    wifi_set_event_handler_cb(&espCfWiFi_callBack);
/*
	enum phy_mode { 
		PHY_MODE_11B = 1, 
		PHY_MODE_11G = 2, 
		PHY_MODE_11N = 3 
	};
*/
    wifi_set_phy_mode(PHY_MODE_11B);
    wifi_set_opmode(mode);

    
    if ( mode == STATION_MODE ) {

    	bzero ( &stationConf.ssid, 32);
	    bzero ( &stationConf.password, 64);
    
	    os_memcpy(&stationConf.ssid, SSID, strlen(SSID));
    	os_memcpy(&stationConf.password, SSID_PASSWORD, strlen(SSID_PASSWORD));
	    wifi_station_set_config(&stationConf);
    	wifi_station_dhcpc_start();
	    
	} 

	    
}


