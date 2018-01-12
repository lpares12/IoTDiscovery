#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

namespace TcpServer {

#define BUFFER_MAX_LENGTH 1024

/**
 * @brief The TcpSession class handles incoming tcp connections once the session
 * has been established. It will read the data sent from the iot device to add it
 * to the discovered devices list.
 */
class TcpSession : public boost::enable_shared_from_this<TcpServer::TcpSession>
{
public:
    /**
     * @brief TcpSession Constructs the session object and inits the socket
     * @param ioService
     *
     * Constructor of the session, will init a socket given the io_service object.
     */
    TcpSession(boost::asio::io_service &ioService);

    /**
     * @brief getSocket Getter of the socket
     * @return socket
     *
     * Getter of the socket.
     */
    boost::asio::ip::tcp::socket& getSocket();
    /**
     * @brief Read data from the socket. The data received should contain the info about
     * the device. v1 only contains the device type (photon, esp8266, rpi,...).
     */
    void start();

private:
    static const short maxLength; // Max length of the incoming data

    uint8_t data_[BUFFER_MAX_LENGTH]; // Variable to store the buffer of data received
    boost::asio::ip::tcp::socket socket_; // Socket of the connection

    /**
     * @brief handleRead
     * @param error
     * @param bytes_transferred
     *
     * Once there is incoming data, this method will read it and add the device to the
     * iot devices list.
     */
    void handleRead(const boost::system::error_code &error, size_t bytes_transferred);

};

/**
 * @brief The ResponseHandler class accepts connections to the tcp server
 * and starts the tcp sessions.
 */
class ResponseHandler
{
public:
    /**
     * @brief ResponseHandler Create object and manage acceptor
     * @param ioService
     * @param port Port for the server
     *
     * Constructor for the class. Links to the io_service, creates
     * the acceptor to accept incoming connections to ip:port and
     * links the object session to the acceptor.
     */
    ResponseHandler(boost::asio::io_service& ioService, short port);

private:

    boost::asio::ip::tcp::acceptor acceptor_; // Acceptor for the connection
    boost::asio::io_service& ioService_; // Io service for the tcp server

    /**
     * @brief startAccept Creates a new tcp session and starts the asynchrounous
     * accept for incoming connections.
     */
    void startAccept();

    /**
     * @brief handleAccept
     * @param newTcpSessionPtr
     * @param error
     *
     * Handle an incoming connection by starting the tcp session.
     */
    void handleAccept(boost::shared_ptr<TcpServer::TcpSession> newTcpSessionPtr, const boost::system::error_code& error);

};

} // namespace TcpServer

#endif // TCP_SERVER_H