#include <boost/asio.hpp>
#include <iostream>
#include <memory_resource>
#include "Exceptions/Exceptions.hpp"
#include "TcpSession/TcpSession.hpp"
#include "BufferType.hpp"

using namespace lib::network;

using IOContext = boost::asio::io_context;
using BoostSocket = boost::asio::ip::tcp::socket;
using Acceptor = boost::asio::ip::tcp::acceptor;
using Endpoint = boost::asio::ip::tcp::endpoint;
using ConstBuffer = boost::asio::const_buffer;
using MutableBuffer = boost::asio::mutable_buffer;

template<size_t buffer_size>
using Buffer = BufferType<buffer_size, 3>;

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

    RequestHeaderType header;

    boost::asio::read(socket,
                      MutableBuffer((char *) &header, REQUEST_HEADER_SIZE),
                      boost::asio::transfer_exactly(REQUEST_HEADER_SIZE));

    ///если посылка входящих данных больше чем буфер
    if (header.Length > buffer_size)
        throw lib::exceptions::BufferOverflow("Переполнение буфера");


    if (header.Length > 0)
        boost::asio::read(socket,
                          MutableBuffer(buffer.begin(), header.Length),
                          boost::asio::transfer_exactly(header.Length));

    return header;
}


inline void SendResponse(BoostSocket &socket, const Response &response) {
    std::array<ConstBuffer, 2> responseData{
            ConstBuffer(&response.Header, RESPONSE_HEADER_SIZE),
            ConstBuffer(response.Data.data(), response.Header.Length)};

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

                TimePoint begin = std::chrono::steady_clock::now();

                ArrayView data{buffer.data(), 0};

                TimePoint end = std::chrono::steady_clock::now();

                if (requestHeader.Type == RequestTypeEnum::W)
                    data = buffer.Decompress(requestHeader.Length);

                auto &service = FindService(requestHeader.EndpointId);

                auto response = service.DoAction(requestHeader.Type, (std::string_view) data);

                SendResponse(socket, response);

                std::cout << requestHeader << '\n'
                          << "Execution time = "
                          << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
                          << "[ns]" << std::endl;

            } catch (const lib::exceptions::BufferOverflow &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (const lib::exceptions::ConnectionTimeout &e) {
                socket.close();
                std::cout << e.Message << std::endl;
            } catch (const lib::exceptions::BaseException &e) {
                Response response(std::string(e.Message), e.Code, -1);
                SendResponse(socket, response);
            } catch (const std::exception &e) {
                socket.close();
                std::clog << "[UNKNOWN EXCEPTION ERROR]\n"
                          << e.what() << std::endl;
            }
            catch (...) {
                socket.close();
                std::clog << "[UNKNOWN ERROR]" << std::endl;
            }
        }
    }
}

inline IService &TcpSession::FindService(ssize_t key) {
    auto item = services_.find(key);

    if (item == services_.end())
        throw lib::exceptions::NotFount("Endpoint not found");

    return *(item->second);
}


/*****************IService member functions*************************/
inline Response IService::ReadData(const std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    ReadValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Read(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteData(const std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = Write(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

inline Response IService::WriteReadData(const std::string_view &data) {

//    std::clog << "\n[VALIDATE]\n" << std::endl;
    WriteReadValidate(data);
//    std::clog << "\n[START ACTION]\n" << std::endl;
    auto response = WriteRead(data);
//    std::clog << "\n[ACTIONS DONE]\n" << std::endl;

    return response;
}

Response IService::Read(const std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::Write(const std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

Response IService::WriteRead(const std::string_view &data) { throw exceptions::NotFount("Данный метод не существует"); }

bool IService::ReadValidate(const std::string_view &data) { return true; }

bool IService::WriteValidate(const std::string_view &data) { return true; }

bool IService::WriteReadValidate(const std::string_view &data) { return false; }

Response IService::DoAction(RequestTypeEnum type, const std::string_view &data) {

    switch (type) {
        case RequestTypeEnum::R:
            return this->ReadData(data);

        case RequestTypeEnum::W:
            return this->WriteData(data);

        case RequestTypeEnum::RW:
            return this->WriteReadData(data);

        default:
            throw lib::exceptions::NotFount("[REQUEST NOT FOUND]");
    }
}