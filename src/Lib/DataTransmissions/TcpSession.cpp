#include <iostream>
#include <memory_resource>
#include "Exceptions/Exceptions.hpp"
#include "TcpSession/TcpSession.hpp"

using namespace lib::network;

using IOContext = boost::asio::io_context;
using BoostSocket = boost::asio::ip::tcp::socket;
using Acceptor = boost::asio::ip::tcp::acceptor;
using Endpoint = boost::asio::ip::tcp::endpoint;
template<size_t buffer_size>
using Buffer = std::array<char, buffer_size>;

constexpr size_t REQUEST_HEADER_SIZE = sizeof(RequestHeaderType);
constexpr size_t RESPONSE_HEADER_SIZE = sizeof(Response::HeaderType);
constexpr uint16_t PORT = TcpSession::PORT;

inline BoostSocket GetConnection(IOContext &ioContext) {
    boost::asio::ip::tcp (*V4)() = boost::asio::ip::tcp::v4;

    BoostSocket socket(ioContext);

    std::clog << "[LISTENING]" << std::endl;

    Acceptor acceptor(ioContext, Endpoint(V4(), PORT));
    acceptor.accept(socket);

    std::clog << "[CONNECTION ACCEPT]" << std::endl;

    return socket;
}

template<size_t buffer_size>
inline std::pair<RequestHeaderType, size_t> ReadData(BoostSocket &socket, Buffer<buffer_size> &buffer) {
    //using namespace boost::asio;

    boost::asio::mutable_buffer (*MakeBuffer)(void *, size_t) = boost::asio::buffer;
    boost::asio::detail::transfer_exactly_t (*TransferExactly)(size_t) = boost::asio::transfer_exactly;

    RequestHeaderType header;

    std::size_t length = boost::asio::read(socket,
                                           MakeBuffer((char *) &header, REQUEST_HEADER_SIZE),
                                           TransferExactly(REQUEST_HEADER_SIZE));

    std::clog << "[HEADER SIZE]: " << REQUEST_HEADER_SIZE << std::endl;

    std::clog << "[REQUEST TYPE]: " << (size_t) header.Type
              << "\n[ENDPOINT]: " << header.EndpointId
              << "\n[DATA LENGTH]: " << header.Length << std::endl;

    ///если посылка входящих данных больше чем буфер
    if (header.Length > buffer_size)
        throw lib::exceptions::BufferOverflow("Переполнение буфера");


    std::size_t readBytes = boost::asio::read(socket,
                                              MakeBuffer(buffer.data(), header.Length),
                                              TransferExactly(header.Length));

    length += readBytes;
    std::cout << "[BYTES RECEIVE]: " << length << '\0' << std::endl;

    std::clog << "[DATA RECEIVED]: " << '|';
    for (size_t i = 0; i < header.Length; ++i) {
        std::clog << (int32_t) buffer[i] << '|';
    }

    std::clog << std::endl;

    return {header, length};
}

template<size_t buffer_size>
inline Response DoAction(IService &service, const RequestHeaderType &header, const Buffer<buffer_size> &dataBuffer) {

    std::string_view data(dataBuffer.data(), header.Length);

    switch (header.Type) {
        case TypeEnum::R:
            return service.ReadData(data);

        case TypeEnum::W:
            return service.WriteData(data);

        case TypeEnum::RW:
            return service.WriteReadData(data);

        default:
            throw lib::exceptions::NotFount("Такой типа запроса не определён библиотекой");
    }
}

inline std::pair<Response::HeaderType, size_t> SendResponse(BoostSocket &socket, const Response &response) {

    boost::asio::mutable_buffer (*MakeBuffer)(void *, size_t) = boost::asio::buffer;
    boost::asio::detail::transfer_exactly_t (*TransferExactly)(size_t) = boost::asio::transfer_exactly;

    auto &header = response.Header;

    size_t length = boost::asio::write(socket,
                                       MakeBuffer((void *) &header, RESPONSE_HEADER_SIZE),
                                       TransferExactly(RESPONSE_HEADER_SIZE));

    if (header.Length > 0)
        length += boost::asio::write(socket,
                                     boost::asio::buffer(response.Data.data(), header.Length),
                                     TransferExactly(header.Length));

    return {response.Header, length};
}

/*****************TcpSession member functions*************************/
void TcpSession::Start() {

    Buffer<BUFFER_SIZE> buffer{};

    IOContext ioContext;

    for (;;) {

        BoostSocket socket = GetConnection(ioContext);

        while (socket.is_open()) {

            try {

                auto[requestHeader, requestLength] = ReadData(socket, buffer);

                if (requestLength != requestHeader.Length + REQUEST_HEADER_SIZE)
                    throw exceptions::TransferError("Проблемы с передачей данных");

                auto &service = FindService(requestHeader.EndpointId);
                auto response = DoAction(service, requestHeader, buffer);

                auto[responseHeader, responseLength] = SendResponse(socket, response);

                if (responseLength != responseHeader.Length + RESPONSE_HEADER_SIZE)
                    throw exceptions::TransferError("Проблемы с передачей данных");

            } catch (lib::exceptions::BufferOverflow &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (lib::exceptions::BaseException &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (const std::exception &e) {
                socket.close();
                std::clog << "[UNKNOW ERROR]" << std::endl;
            }
            catch (...) {
                socket.close();
                std::clog << "[UNKNOW ERROR]" << std::endl;
            }
        }
    }
}

TcpSession &TcpSession::GetInstance() {
    static TcpSession network;
    return network;
}

inline IService &TcpSession::FindService(ssize_t key) {
    auto item = services_.find(key);

    if (item == services_.end())
        throw lib::exceptions::NotFount("Эндпоинт не найден");

    return *(item->second);
}


/*****************IService member functions*************************/
inline Response IService::ReadData(std::string_view &data){

    EASY_FUNCTION(profiler::colors::Magenta); // Magenta block with name "Read"
    EASY_BLOCK("Calculating sum"); // Begin block with default color == Amber100

    std::clog << "\n[VALIDATE]\n" << std::endl;
    ReadValidate(data);
    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Read(data);
    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteData(std::string_view &data){

    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteValidate(data);
    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Write(data);
    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteReadData(std::string_view &data){

    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteReadValidate(data);
    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = WriteRead(data);
    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

Response IService::Read(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::Write(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::WriteRead(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

bool IService::ReadValidate(std::string_view &data) { return true; }

bool IService::WriteValidate(std::string_view &data) { return true; }

bool IService::WriteReadValidate(std::string_view &data) { return false; }



