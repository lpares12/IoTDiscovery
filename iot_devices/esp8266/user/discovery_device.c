#include "discovery_device.h"

#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"

static const char * msg_discovery = "Discovery"; // Discovery message string
static const char * msg_reply_discovery = "esp8266"; // Discovery message reply string

// Udp variables
static const uint8_t MCAST_ADDRESS[4] = {239,255,0,1}; // Address for the multicast server
static const int MCAST_PORT = 20452; // Port for the multicast endpoint
static struct espconn multicast_conn; // Struct holding the discovery multicast connection values
static esp_udp multicast_udp_conn; // Struct holding the udp connection values

// Tcp variables
static const int TCP_REMOTE_PORT = 10011; // Port to connect for the discovery response
static struct espconn tcp_conn; // Struct holding the discovery response connection values
static esp_tcp response_tcp_conn; // Struct holding the tcp connection values

static void ICACHE_FLASH_ATTR tcp_discon_cb(void *arg)
{   
    os_printf("Disconnected from server.\r\n");
}

static void ICACHE_FLASH_ATTR send_discovery_reply(struct espconn * ptrespconn)
{
    char *reply = (char *) msg_reply_discovery;
    espconn_sent(ptrespconn, (uint8 *) reply, os_strlen(msg_reply_discovery)); // TODO: Change to espconn_send in higher SDKs

    os_printf("Sent response\n");
}

static void ICACHE_FLASH_ATTR tcp_connected_cb(void *arg)
{
    struct espconn *ptrespconn = (struct espconn *) arg;

    os_printf("Connected to %d.%d.%d.%d\n", IP2STR(ptrespconn->proto.udp->remote_ip));

    send_discovery_reply(ptrespconn);
}

static void ICACHE_FLASH_ATTR prepare_for_reply(const struct espconn * const ptrespconn)
{
    tcp_conn.type = ESPCONN_TCP;
    tcp_conn.state = ESPCONN_NONE;
    os_memcpy(response_tcp_conn.remote_ip, ptrespconn->proto.udp->remote_ip, sizeof(uint8_t)*4);
    response_tcp_conn.remote_port = TCP_REMOTE_PORT;
    response_tcp_conn.local_port = espconn_port(); // Set local port

    tcp_conn.proto.tcp = &response_tcp_conn;

    espconn_regist_connectcb(&tcp_conn, tcp_connected_cb);
    espconn_regist_disconcb(&tcp_conn, tcp_discon_cb);
    espconn_connect(&tcp_conn);
}

static void ICACHE_FLASH_ATTR udp_recv(void *arg, char *data, unsigned short length)
{
    struct espconn *ptrespconn = (struct espconn *) arg; // retrieve the sender info

    os_printf("Packet received from %d.%d.%d.%d\n", IP2STR(ptrespconn->proto.udp->remote_ip));

    if((os_strlen(msg_discovery)) == length)
    {
        if(os_strncmp(msg_discovery, data, strlen(msg_discovery)) == 0)
        {
            os_printf("Discovery received\n");

            prepare_for_reply(ptrespconn);
        }
    }
}

void ICACHE_FLASH_ATTR setup_multicast_receiver()
{
    struct ip_info local_ip_info;
    ip_addr_t multicast_group_ip;
    uint8_t error;

    multicast_udp_conn.local_port = MCAST_PORT;

    multicast_conn.type = ESPCONN_UDP;
    multicast_conn.state = ESPCONN_NONE;
    multicast_conn.proto.udp = &multicast_udp_conn;

    wifi_get_ip_info(STATION_IF, &local_ip_info);
    IP4_ADDR(&multicast_group_ip, MCAST_ADDRESS[0],MCAST_ADDRESS[1],MCAST_ADDRESS[2],MCAST_ADDRESS[3]);

    espconn_igmp_join(&local_ip_info.ip, (struct ip_addr *) &multicast_group_ip);

    error = espconn_regist_recvcb(&multicast_conn, udp_recv);
    if(error != 0) os_printf("Error in espconn_register_recvcb: %d\n", error);
    error = espconn_create(&multicast_conn);
    if(error != 0) os_printf("Error in espconn_create: %d\n", error);

    os_printf("Multicast client initialised\n");
}