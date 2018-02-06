/**
* @file
* @author  Lluc Pares
* @date    18/01/2018
* @brief   Connect to access point with ESP8266
*
* This file contains the API to connect to a given access point.
**/
#ifndef NETWORK_STAT_H
#define NETWORK_STAT_H

#include "user_interface.h"

/**
 * @brief Check connection status.
 */
static void ICACHE_FLASH_ATTR network_check_connection(void *arg);

/**
 * @brief Connects to a network.
 * @param net_ssid SSID of the network to connect
 * @param net_password pass of the network to connect
 *
 * Connects to a network using the configuration saved in flash
 * if exists, or the custom configuration.
 */
void ICACHE_FLASH_ATTR network_connect(const char *net_ssid, const char *net_password);

#endif // NETWORK_STAT_H
