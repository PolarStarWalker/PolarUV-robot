#include <boost/asio.hpp>
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
inline RequestHeaderType ReadData(BoostSocket &socket, Buffer<buffer_size> &buffer) {

    boost::asio::mutable_buffer (*MakeBuffer)(void *, size_t) = boost::asio::buffer;
    boost::asio::detail::transfer_exactly_t (*TransferExactly)(size_t) = boost::asio::transfer_exactly;

    RequestHeaderType header;

    boost::asio::read(socket,
                      MakeBuffer((char *) &header, REQUEST_HEADER_SIZE),
                      TransferExactly(REQUEST_HEADER_SIZE));

//    std::clog << "[HEADER SIZE]: " << REQUEST_HEADER_SIZE << std::endl;
//
//    std::clog << "[REQUEST TYPE]: " << (size_t) header.Type
//              << "\n[ENDPOINT]: " << header.EndpointId
//              << "\n[DATA LENGTH]: " << header.Length << std::endl;

    ///если посылка входящих данных больше чем буфер
    if (header.Length > buffer_size)
        throw lib::exceptions::BufferOverflow("Переполнение буфера");


    if (header.Length > 0)
        boost::asio::read(socket,
                          MakeBuffer(buffer.data(), header.Length),
                          TransferExactly(header.Length));


//    std::cout << "[BYTES RECEIVE]: " << length << '\0' << std::endl;
//
//    std::clog << "[DATA RECEIVED]: " << '|';
//    for (size_t i = 0; i < header.Length; ++i) {
//        std::clog << (int32_t) buffer[i] << '|';
//    }

//    std::clog << std::endl;

    return header;
}


inline void SendResponse(BoostSocket &socket, const Response &response) {
    using namespace boost::asio;

    std::array<boost::asio::const_buffer, 2> responseData{
            boost::asio::const_buffer(&response.Header, RESPONSE_HEADER_SIZE),
            boost::asio::const_buffer(response.Data.data(), response.Header.Length)};

    auto &header = response.Header;

    boost::asio::write(socket,
                       responseData,
                       boost::asio::transfer_exactly(RESPONSE_HEADER_SIZE + response.Header.Length));
}


/*****************TcpSession member functions*************************/
void TcpSession::Start() {
    using TimePoint = std::chrono::steady_clock::time_point;

    Buffer<BUFFER_SIZE> buffer{};

    IOContext ioContext;

    for (;;) {

        BoostSocket socket = GetConnection(ioContext);

        while (socket.is_open()) {

            try {

                auto requestHeader = ReadData(socket, buffer);

                TimePoint requestBegin = std::chrono::steady_clock::now();
                auto &service = FindService(requestHeader.EndpointId);
                auto response = service.DoAction(requestHeader, buffer);

                SendResponse(socket, response);

                TimePoint end = std::chrono::steady_clock::now();
                std::cout << "Request Time = "
                          << std::chrono::duration_cast<std::chrono::microseconds>(end - requestBegin).count()
                          << "[us]" << std::endl;

            } catch (lib::exceptions::BufferOverflow &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (lib::exceptions::BaseException &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (const std::exception &e) {
                socket.close();
                std::clog << "[UNKNOW ERROR] " << e.what() << std::endl;
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
        throw lib::exceptions::NotFount("Endpoint not found");

    return *(item->second);
}


/*****************IService member functions*************************/
inline Response IService::ReadData(std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    ReadValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Read(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteData(std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Write(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteReadData(std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteReadValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = WriteRead(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

Response IService::Read(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::Write(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::WriteRead(std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

bool IService::ReadValidate(std::string_view &data) { return true; }

bool IService::WriteValidate(std::string_view &data) { return true; }

bool IService::WriteReadValidate(std::string_view &data) { return false; }



