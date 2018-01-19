#include "network_station.h"

#include "osapi.h"
#include "os_type.h"
#include "ip_addr.h"
#include "espconn.h"

// Variables and defines for connection
#define SSID_MAX_LENGTH 32 // Must be a macro to init arrays with this number
static const uint32_t NET_CONNECTION_TIMER_MS = 500;
static os_timer_t network_connection_timer;

static void ICACHE_FLASH_ATTR network_check_connection(void *arg)
{
	uint8_t end = 0;
    uint8_t net_status;

    os_timer_disarm(&network_connection_timer);

    switch((net_status = wifi_station_get_connect_status()))
    {
        case STATION_IDLE:
            os_printf("Status IDLE\n");
            break;
        case STATION_CONNECTING:
            os_printf("Connecting to access point\n");
            break;
        case STATION_GOT_IP:
            os_printf("ESP8266 IP: ");
            struct ip_info ip_info;
            wifi_get_ip_info(STATION_IF, &ip_info);
            os_printf("%d.%d.%d.%d\n", IP2STR(&ip_info.ip));
            end = 1;
            break;
        default:
            os_printf("Error in connection %d\n", net_status);
            end = 1;
    }

    // If we haven't connected yet and there is no error in the communication
    if(end == 0)
    {
        os_timer_setfn(&network_connection_timer, (os_timer_func_t *) network_check_connection, NULL);
        os_timer_arm(&network_connection_timer, NET_CONNECTION_TIMER_MS, 0);
    }
}

void ICACHE_FLASH_ATTR network_connect(const char *net_ssid, const char *net_password)
{
    char net_ssid_str[SSID_MAX_LENGTH] = "";

    struct station_config stationConf;

    bool configFound = wifi_station_get_config(&stationConf);

    if(!configFound)
    {
        os_memset(stationConf.ssid, 0, SSID_MAX_LENGTH);
        os_memset(stationConf.password, 0, 64);
        stationConf.bssid_set = 0; // make sure it isn't looking for an specific MAC Address

        os_memcpy(&stationConf.ssid, net_ssid, os_strlen(net_ssid));
        os_memcpy(&stationConf.password, net_password, os_strlen(net_password));
        wifi_station_set_config(&stationConf); // This saves stationConf to flash, use set_config_current for not saving

        os_printf("Configuration not found\n");
    }
    else
    {    
        os_memcpy(&net_ssid_str, stationConf.ssid, SSID_MAX_LENGTH);
        os_printf("Configuration found, connecting to ssid: %s\n", net_ssid_str);
    }

    wifi_station_connect();

    os_timer_disarm(&network_connection_timer);
    os_timer_setfn(&network_connection_timer, (os_timer_func_t *) network_check_connection, NULL);
    os_timer_arm(&network_connection_timer, NET_CONNECTION_TIMER_MS, 0);
}