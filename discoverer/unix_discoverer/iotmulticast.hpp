#ifndef IOT_MULTICAST_H
#define IOT_MULTICAST_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace IotMulticast {

#define MULTICAST_ADDRESS "239.255.0.1" // Address for the multicast endpoint
#define MULTICAST_PORT 20452 // Port for the multicast endpoint

/**
 * @brief The MulticastSender class is used to send the multicast messages to the
 * IOT devices.
 */
class MulticastSender
{
public:
    /**
     * @brief Constructor of the class, inits an ioService, socket and endpoint to
     * the multicast address:port.
     */
    MulticastSender();

    /**
     * @brief Send a discovery message to the multicast address:port
     */
    void discoverDevices(std::string message);

    /**
     * @brief Close the socket to stop sending the multicast messages
     */
    void close();

private:
    boost::asio::io_service ioService_; // InputOutput service for the multicasting
    boost::asio::ip::udp::socket multicastSocket_; // Socket for the multicast messages
    boost::asio::ip::udp::endpoint endpoint_; // Endpoint for the multicast address:port
};

} // namespace IotMulticast

#endif // IOT_MULTICAST_H