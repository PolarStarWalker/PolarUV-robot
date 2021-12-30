#include "TcpSession/TcpSession.hpp"

#include <iostream>

using namespace boost::asio::ip;

TcpSession::TcpSession() :
        _ioContext(),
        _socket(_ioContext){
}

TcpSession::~TcpSession() {
    _ioContext.stop();
}

TcpSession &TcpSession::GetInstance() {
    static TcpSession network;
    return network;
}

void TcpSession::Start() {
    char buffer[MAX_BUFFER_SIZE]{};

    boost::asio::io_context *ioContext = &_ioContext;

    auto work = boost::asio::require(_ioContext.get_executor(),
                                     boost::asio::execution::outstanding_work.tracked);

    _thread = std::thread([ioContext]() { ioContext->run(); });
    _thread.detach();

    for (;;) {
        tcp::acceptor acceptor(_ioContext, tcp::endpoint(tcp::v4(), PORT));
        acceptor.accept(_socket);

        while (_socket.is_open()) {
            size_t length = 0;

            try {

                auto foo =
                        _socket.async_read_some(boost::asio::buffer(buffer, MAX_BUFFER_SIZE),
                                                boost::asio::use_future);

                length = foo.get();

                std::cout << buffer << std::endl;

                std::memset(buffer, 0, sizeof(buffer));

            } catch (std::exception &e) {
                _socket.close();
            }

            if (!length)
                _socket.close();
        }
    }
}

void TcpSession::TcpPrint() {

    char buffer[MAX_BUFFER_SIZE]{};

    boost::asio::io_context *ioContext = &_ioContext;

    auto work = boost::asio::require(_ioContext.get_executor(),
                                     boost::asio::execution::outstanding_work.tracked);

    _thread = std::thread([ioContext]() { ioContext->run(); });
    _thread.detach();

    for (;;) {
        tcp::acceptor acceptor(_ioContext, tcp::endpoint(tcp::v4(), PORT));
        acceptor.accept(_socket);

        while (_socket.is_open()) {
            size_t length = 0;

            try {

                auto foo =
                        _socket.async_read_some(boost::asio::buffer(buffer, MAX_BUFFER_SIZE),
                                                boost::asio::use_future);

                length = foo.get();

                std::cout << buffer << std::endl;

                std::memset(buffer, 0, sizeof(buffer));

            } catch (std::exception &e) {
                _socket.close();
            }

            if (!length)
                _socket.close();
        }
    }
}

void TcpSession::AddService(const IService * service) {
    _workers[service->_serviceId] = service;
}



