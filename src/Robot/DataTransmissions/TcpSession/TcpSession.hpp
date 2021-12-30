#ifndef ROBOT_TCPSESSION_HPP
#define ROBOT_TCPSESSION_HPP

#include <boost/asio.hpp>
#include <unordered_map>

struct Request {
    Request(size_t serviceId, const char* data, size_t length): _serviceId(serviceId), _data(data), _length(length){}

    const char* _data;
    const size_t _length;
    const size_t _serviceId;
};

struct Responce {
    enum StatusCode{
        Ok,
        SomeTrouble
    };

    std::string data;
};

class IService{
public:
    explicit IService(size_t serviceId) : _serviceId(serviceId){};

    virtual Responce Handle(const Request& request) const = 0;

    const size_t _serviceId;
};

class TcpSession {
private:
    const static size_t PORT = 2022;
    constexpr static size_t MAX_BUFFER_SIZE = 512;

public:
    TcpSession(const TcpSession &) = delete;

    TcpSession(TcpSession &&) = delete;

    static TcpSession &GetInstance();

    [[noreturn]]
    void Start();

    [[noreturn]]
    [[deprecated("Test")]]
    void TcpPrint();

    void AddService(const IService* service);

private:
    TcpSession();
    ~TcpSession();

    std::unordered_map<size_t, const IService *> _workers;

    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::socket _socket;

    std::thread _thread;
};

#endif
