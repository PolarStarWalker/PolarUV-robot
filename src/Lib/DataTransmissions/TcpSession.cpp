#include <iostream>
#include <memory_resource>
#include "Exceptions/Exceptions.hpp"
#include "TcpSession/TcpSession.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace lib::network;

constexpr size_t REQUEST_HEADER_SIZE = sizeof(RequestHeaderType);
constexpr size_t RESPONSE_HEADER_SIZE = sizeof(Response::HeaderType);
constexpr size_t BUFFER_SIZE = TcpSession::BUFFER_SIZE;

tcp::socket Listen(io_context &ioContext) {

    tcp::socket socket(ioContext);

    std::clog << "[LISTENING]" << std::endl;

    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), TcpSession::PORT));
    acceptor.accept(socket);

    std::clog << "[CONNECTION ACCEPT]" << std::endl;

    return socket;
}

template<size_t bufferSize>
std::pair<RequestHeaderType, size_t> ReadData(tcp::socket &socket, std::array<char, bufferSize> &dataBuffer) {

    RequestHeaderType header;

    std::size_t length = read(socket,
                              buffer((char *) &header, REQUEST_HEADER_SIZE),
                              transfer_exactly(REQUEST_HEADER_SIZE));

    std::clog << "[HEADER SIZE]: " << REQUEST_HEADER_SIZE << std::endl;

    std::clog << "[REQUEST TYPE]: " << (size_t) header.Type
              << "\n[ENDPOINT]: " << header.EndpointId
              << "\n[DATA LENGTH]: " << header.Length << std::endl;

    ///если посылка входящих данных больше чем буфер
    if (header.Length > bufferSize) {
        //ToDo экзепшен как бэ тут не помешал бы
        std::clog << "Sempai, I'm full" << std::endl;
    }

    std::size_t readBytes = read(socket,
                                 buffer(dataBuffer.data(), header.Length),
                                 transfer_exactly(header.Length));

    length += readBytes;
    std::cout << "[BYTES RECEIVE]: " << length << '\0' << std::endl;

    std::clog << "[DATA RECEIVED]: " << '|';
    for (size_t i = 0; i < header.Length; ++i) {
        std::clog << (int32_t) dataBuffer[i] << '|';
    }

    std::clog << std::endl;

    return {header, length};
}

template<size_t bufferSize>
Response DoAction(IService &service, const RequestHeaderType &header, const std::array<char, bufferSize> &dataBuffer) {

    std::string_view data(dataBuffer.data(), header.Length);

    auto run = [&](Response(IService::*function)(std::string_view& data)) {

        std::clog << "\n[START ACTION]" << '\n' << std::endl;
        Response response = (service.*function)(data);
        std::clog << "\n[ACTIONS DONE]" << '\n' << std::endl;

        return response;
    };

    switch (header.Type) {
        case TypeEnum::R:
            return run(&IService::Read);

        case TypeEnum::W:
            return run(&IService::Write);

        case TypeEnum::RW:
            return run(&IService::Read);

        default:
            throw lib::exceptions::NotFount("Такой типа запроса не определён библиотекой");
    }
}

void SendResponse(tcp::socket &socket, const Response &response) {
    auto &header = response.Header;

    write(socket,
          buffer((char *) &header, RESPONSE_HEADER_SIZE),
          transfer_exactly(RESPONSE_HEADER_SIZE));

    write(socket,
          buffer(response.Data.c_str(), header.Length),
          transfer_exactly(header.Length));
}

/*****************TcpSession member functions*************************/
void TcpSession::Start() {

    std::array<char, BUFFER_SIZE> buffer{};

    io_context ioContext;

    for (;;) {

        tcp::socket socket = Listen(ioContext);

        while (socket.is_open()) {

            try {

                auto[requestHeader, length] = ReadData(socket, buffer);

                if (length != requestHeader.Length + REQUEST_HEADER_SIZE)
                    throw exceptions::NotFount("Проблемы с передачей данных");

                auto &service = FindService(requestHeader);
                auto response = DoAction(service, requestHeader, buffer);

                SendResponse(socket, response);

            } ///ToDo: бработка экзепшенов
            catch (...) {
                socket.close();
                std::clog << "[CONNECTION LOST]" << std::endl;
            }
        }
    }
}

TcpSession &TcpSession::GetInstance() {
    static TcpSession network;
    return network;
}

void TcpSession::AddService(std::unique_ptr<IService> &&service) {
    services_[service->serviceId_] = std::move(service);
}

IService &TcpSession::FindService(const RequestHeaderType &header) {
    auto item = services_.find(header.EndpointId);

    if (item == services_.end())
        throw lib::exceptions::NotFount("Эндпоинт не найден");

    return *(item->second);
}


/*****************IService member functions*************************/
Response IService::Read(string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::Write(string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::ReadWrite(string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }



