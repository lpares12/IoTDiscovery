#include <iostream>
#include <unistd.h>
#include "iotmulticast.hpp"
#include "tcpserver.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

static const short waitTime = 4; // seconds

static const short tcpPort = 10011; // Port for the tcp connection

int main()
{
    try
    {
        // Init io_service for the tcp server
        boost::asio::io_service tcpService;
        // Create a new work for the tcp service
        boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(tcpService));
        // Construct the response handler object
        TcpServer::ResponseHandler tcpServer(tcpService, tcpPort);
        // Run the tcp server in its own thread to continue with the program execution
        boost::thread bt(boost::bind(&boost::asio::io_service::run, &tcpService)); // to run the server we execute tcpService.run() in another thread
        std::cout << "Tcp server initialised\n";

        IotMulticast::MulticastSender discoverySender;
        // Send the multicast discovery message to all iot devices
        discoverySender.discoverDevices("Discovery");
        discoverySender.close();
        std::cout << "Discovery Sent\n";

        // Wait X seconds for all the devices to respond
        sleep(waitTime);

        // Stop the server
        tcpService.stop(); // stop the tcp service (will make the thread end its execution)
        bt.join(); // wait for thread to join
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
