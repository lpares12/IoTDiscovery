#include "tcpserver.hpp"

#include <iostream>

using namespace TcpServer;

const short maxLength = 1024; // Max length of the incoming data

TcpSession::TcpSession(boost::asio::io_service &ioService) : socket_(ioService)
{
}

boost::asio::ip::tcp::socket& TcpSession::getSocket()
{
    return socket_;
}

void TcpSession::start()
{
    socket_.async_read_some(boost::asio::buffer(data_,100), 
                            boost::bind(&TcpSession::handleRead, shared_from_this(), 
                            boost::asio::placeholders::error, 
                            boost::asio::placeholders::bytes_transferred));
}

void TcpSession::handleRead(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error)
    {
        std::cout << "New iot client: " << socket_.remote_endpoint().address().to_string();
        data_[bytes_transferred] = '\0'; // to print array until \0
        std::cout << " is a " << data_ << std::endl;

        // TODO: Check if we could close the socket or we want to continue reading
        // Continue reading until error appears (connection closed by endpoint)
        start();
    }
    else
    {
        // If error is not eof, check what we did wrong
        if(error != boost::asio::error::eof) std::cout << "Error: " << error.message() << std::endl;
    }
}


ResponseHandler::ResponseHandler(boost::asio::io_service& ioService, short port) :
        ioService_(ioService),
        acceptor_(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    startAccept();
}

void ResponseHandler::startAccept()
{
    boost::shared_ptr<TcpSession> newTcpSessionPtr(new TcpSession(ioService_));
    acceptor_.async_accept(newTcpSessionPtr->getSocket(),
                           boost::bind(&ResponseHandler::handleAccept, this, newTcpSessionPtr,
                           boost::asio::placeholders::error));
}

void ResponseHandler::handleAccept(boost::shared_ptr<TcpSession> newTcpSessionPtr, const boost::system::error_code& error)
{
    if(!error)
    {
        newTcpSessionPtr->start();
    }
    else
    {
        std::cout << "New session with error\n";
    }

    startAccept(); // Accept new connections
}
