/**
* @file
* @author  Lluc Pares
* @date    18/01/2018
* @brief   ESP8266 example to handle the discovery messages
*
* This file contains an example for ESP8266 devices to handle the
* IoTDiscovery message.
**/

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "user_interface.h"

/**
 * @brief Callback for tcp disconnection.
 */
static void ICACHE_FLASH_ATTR tcp_discon_cb(void *arg);

/**
 * @brief Send discovery reply to tcp server
 * @param ptrespconn Struct holding the discovery response connection values
 */
static void ICACHE_FLASH_ATTR send_discovery_reply(struct espconn * ptrespconn);

/**
 * @brief Callback for tcp connection.
 */
static void ICACHE_FLASH_ATTR tcp_connected_cb(void *arg);

/**
 * @brief Prepare the tcp connection for replying the discovery message.
 * @param ptrespconn Struct holding the discovery message (udp multicast) connection values
 */
static void ICACHE_FLASH_ATTR prepare_for_reply(const struct espconn * const ptrespconn);

/**
 * @brief Callback for when an udp packet is received.
 * @param arg Contains a pointer to espconn struct
 * @param data Contains data received
 * @param length Contains the length of the data
 */
static void ICACHE_FLASH_ATTR udp_recv(void *arg, char *data, unsigned short length);

/**
 * @brief Init udp variables and join multicast group.
 *
 * This is the entry point of this API.
 */
void ICACHE_FLASH_ATTR setup_multicast_receiver();

#endif // DISCOVERY_H
