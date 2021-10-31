#include "./Socket/Socket.hpp"

Socket::Socket(uint16_t port) {

    _serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, INTERNAL_SERVER_IP, &(_serverAddress.sin_addr));

    if (bind(_serverSocketDescriptor, (sockaddr *) &(_serverAddress), _serverAddressLength) == -1)
        throw std::exception();

}

Socket::~Socket() {
    close(_serverSocketDescriptor);
}

///function that listen in blocking mode
int Socket::Listen() {

    std::cout << "Server is listening\n";

    if (listen(_serverSocketDescriptor, SOMAXCONN) == -1) {
        return (-1);
    }

    _clientSocketDescriptor = accept(_serverSocketDescriptor,
                                           (sockaddr *) &(_clientAddress),
                                           &(_clientAdressLength));

    _isOnline = true;

    return (1);
}

ssize_t Socket::RecvDataLen(char *msg, size_t len) {
    ssize_t result = recvall(_clientSocketDescriptor, msg, len, 0);
    return (result);
}

ssize_t Socket::SendDataLen(char *msg, size_t len) {
    ssize_t result = sendall(_clientSocketDescriptor, msg, len, 0);
    return (result);
}

ssize_t Socket::recvall(int socketDescriptor, char *buf, size_t len, int flags) {

    //Структура для ожидания
    struct pollfd pfd{};
    pfd.fd = socketDescriptor;
    pfd.events = POLLIN;
    //само ожидание
    ssize_t result = poll(&pfd, 1, 1000);

    if (result == -1)
        _isOnline = false;

    ssize_t n = recv(socketDescriptor, buf, len, flags);

    if (n != len)
        _isOnline = false;

    return n;
}

ssize_t Socket::sendall(int socketDescriptor, char *buf, size_t len, int flags) {

    if (!_isOnline)
        return -1;

    ssize_t n = send(socketDescriptor, buf, len, flags);

    if (n != len)
        _isOnline = false;

    return n;
}

bool Socket::IsOnline() const {
    return _isOnline;
}
