#include "TcpSession/TcpSession.hpp"

#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace lib::network;
constexpr size_t HEADER_SIZE = sizeof(HeaderType);

HeaderType HEADER;
std::array<char, TcpSession::BUFFER_SIZE> BUFFER;


TcpSession::TcpSession() :
        _ioContext() {}

TcpSession::~TcpSession() {
    _ioContext.stop();
}

TcpSession &TcpSession::GetInstance() {
    static TcpSession network;
    return network;
}

tcp::socket Listen(io_context &ioContext) {

    tcp::socket socket(ioContext);

    std::clog << "listening" << std::endl;

    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), TcpSession::PORT));
    acceptor.accept(socket);

    std::clog << "accept" << std::endl;

    return socket;
}

size_t ReadData(tcp::socket &socket) {
    std::size_t length = read(socket,
                              buffer((char *) &HEADER, HEADER_SIZE),
                              transfer_exactly(HEADER_SIZE));

    std::cout << "Длинна заголовочника " << HEADER_SIZE << std::endl;

    std::clog << "Тип запроса: " << (size_t) HEADER.Type
              << "\nЭендпоинт: " << HEADER.EndpointId
              << "\nДлина сообщения: " << HEADER.Length << std::endl;

    if (HEADER.Length > BUFFER.size()) {
        //ToDo экзепшен как бэ тут не помешал бы
        std::clog << "Sempai, I'm full" << std::endl;
    }

    std::size_t readed = read(socket,
                  buffer(BUFFER, HEADER.Length),
                  transfer_exactly(HEADER.Length));

    BUFFER[readed] = 0;

    return length + readed;
}

void TcpSession::AddService(std::shared_ptr<IService> &&service) {
    services_[service->_serviceId] = service;
}


void TcpSession::Start() {
    for (;;) {

        tcp::socket socket = Listen(_ioContext);

        while (socket.is_open()) {

            try {

                auto length = ReadData(socket);

                std::cout << "Байт Получено" << length << '\n' << std::endl;

                auto &service = *services_.find(HEADER.EndpointId)->second;
                service.Write(std::string_view(BUFFER.data(), HEADER.Length));

                std::clog << "[ACTIONS DONE]" << std::endl;

            } catch (std::exception &e) {
                socket.close();
                std::clog << "[CONNECTION LOST]" << std::endl;
            }
        }
    }
}

Response IService::Read(std::string_view data) { return {"", Response::BadRequest}; }

Response IService::Write(std::string_view data) { return {"", Response::BadRequest}; }

Response IService::ReadWrite(std::string_view data) { return {"", Response::BadRequest}; }



