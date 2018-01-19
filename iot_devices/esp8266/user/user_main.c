// sudo python -m serial.tools.miniterm /dev/ttyUSB0 76800

#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_config.h"

#include "config.h"
#include "debug.h"
#include "discovery_device.h"
#include "network_station.h"

#define SSID_MAX_LENGTH 32

static os_timer_t connected_timer; // timer for checking connection status

static void ICACHE_FLASH_ATTR done_connected(void)
{
    if(wifi_station_get_connect_status() == STATION_GOT_IP)
    {
        setup_multicast_receiver();
        os_timer_disarm(&connected_timer);
    }
}

/**
 * @brief Calls to connect to network
 * should only be called after system init done, so
 * this will be a callback of system_init_done_cb()
 */
static void ICACHE_FLASH_ATTR init_done(void)
{
    os_timer_disarm(&connected_timer);
    os_timer_setfn(&connected_timer, (os_timer_func_t *) done_connected, NULL);
    os_timer_arm(&connected_timer, 100, 1);

    network_connect(NET_SSID, NET_PASSWORD);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init(void)
{
    init_debug_blink();

    // Set wifi as station mode
    wifi_set_opmode(STATION_MODE);

    // Call init_done method after system init is done
    system_init_done_cb(init_done);
}
