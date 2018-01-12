#include "iotmulticast.hpp"

using namespace IotMulticast;

MulticastSender::MulticastSender() : ioService_(), multicastSocket_(ioService_, boost::asio::ip::udp::v4()),
                        endpoint_(boost::asio::ip::address::from_string(MULTICAST_ADDRESS), MULTICAST_PORT)
{
}

void MulticastSender::discoverDevices(std::string message)
{
    multicastSocket_.send_to(boost::asio::buffer(message, message.size()), endpoint_);
}

void MulticastSender::close()
{
    multicastSocket_.close();
    ioService_.stop();
}