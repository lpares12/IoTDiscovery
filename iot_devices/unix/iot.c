/**
* @file
* @author  Lluc Pares
* @date    13/01/2018
* @brief   Unix example to handle the discovery messages
*
* This file contains an example for unix devices to handle the
* IoTDiscovery message.
**/

// cc iot.c -o iot
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MULTICAST_ADDRESS "239.255.0.1" // Address for the multicast endpoint
#define MULTICAST_PORT 20452 // Port for the multicast endpoint

#define TCP_PORT 10011 // Port for the tcp remote endpoint/server

/**
 * @brief This method create a socket to listen to a multicast group
 * @param sock Pointer to the socket file descriptor
 * @param addr Pointer to the sockaddr_in structure for the udp communication
 * 
 * This method creates and sets the options to a socket to listen to
 * the discovery multicast group.
 */
void createMulticastSocket(int *sock, struct sockaddr_in *addr)
{
	// Create socket
	if((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket error\n");
		exit(1);
	}

	// Allow multiple sockets to use the same port
	u_int yes = 1;
	if(setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		perror("Error allowing multiple uses of port\n");
		exit(1);
	}

	// Fill addr info
	memset(addr, 0, sizeof(*addr)); // Set all memory to 0
	addr->sin_family = AF_INET; // Link it to ipv4 address family
	addr->sin_addr.s_addr = htonl(INADDR_ANY); // Allow incoming packets from any interface
	addr->sin_port = htons(MULTICAST_PORT); // Allow incoming packets to specified port
	// Binding to receive address
	if(bind(*sock, (struct sockaddr *) addr, sizeof(*addr)) < 0)
	{
		perror("Error binding\n");
		exit(1);
	}

	// Set socket options to request kernel to subscribe to a multicast group
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS); // Set multicast group ip
	mreq.imr_interface.s_addr = htonl(INADDR_ANY); // Allow incoming packets from any interface
	if(setsockopt(*sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("Error setting socket options\n");
		exit(1);
	}
}

/**
 * @brief Reply to the discovery message given an endpoint
 * @param sin_addr Address of the tcp server
 *
 * This method connects to the endpoint which sent the discovery
 * message and send info about the device.
 */
int replyDiscovery(struct in_addr sin_addr)
{
	int tcp_sock;
	struct sockaddr_in server_addr;

	// Create socket
	if((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error creating tcp socket\n");
		return -1;
	}

	// Fill server information
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = sin_addr;
	server_addr.sin_port = htons(TCP_PORT);

	// Try a connection
	if(connect(tcp_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		perror("Error connecting to tcp server\n");
		return 1;
	}

	// Send device info to tcp server
	char buffer[] = "unix-iot"; // We will just send a message saying that this is a unix iot device
	if(write(tcp_sock, buffer, sizeof(buffer)) == -1)
	{
		perror("There was an error sending the text\n");
		return 1;
	}
}

int main(int argc, const char* argv[])
{
	int sock; // Socket for the multicast communication
	struct sockaddr_in addr; // Address for the multicast endpoint

	createMulticastSocket(&sock, &addr);

	// Program loop of the IoT device
	while(1)
	{
		int addrlen = sizeof(addr);
		char buffer[1024];
		int recv_bytes = 0;

		// Receive a packet
		if((recv_bytes = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&addr, &addrlen)) < 0)
		{
			perror("Error receiving from socket\n");
			exit(1);
		}

		// Get IP address of the sender
		char ip_str[INET_ADDRSTRLEN];
		const char* result = inet_ntop(AF_INET, &(addr.sin_addr), ip_str, sizeof(ip_str));
		if(result == 0) {
			perror("Error converting sender ip to string\n");
			exit(1);
		}

		printf("Discovery packet received from: %s\n", ip_str);

		// Connect to the tcp server to reply the discovery message
		replyDiscovery(addr.sin_addr);
	}

	return 0;
}