#include "Socket.hpp"

/// Function that create server socket and bind it
int Socket::MakeServerSocket(uint16_t port) {

    this->_socketRole = Server;

    this->_serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, INTERNAL_SERVER_IP, &(this->_serverAddress.sin_addr));

    if (bind(this->_serverSocketDescriptor, (sockaddr*)&(this->_serverAddress), this->_serverAddressLength) == -1) {
        return(-1);
    }

    return(1);
}
///function that listen in blocking mode
int Socket::Listen() {
#ifndef NDEBUG
    std::cout << "Server is listening\n";
#endif
    if (listen(this->_serverSocketDescriptor, SOMAXCONN) == -1) {
        return(-1);
    }

    this->_clientSocketDescriptor = accept(this->_serverSocketDescriptor, (sockaddr*)&(this->_clientAddress), &(this->_clientAdressLength));

    this->_connectionStatus = true;

    return(1);
}

int Socket::MakeConnection(uint16_t port) {

    this->_socketRole = Client;
    
    this->_serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, EXTERNAL_SERVER_IP, &(this->_serverAddress.sin_addr));
    this->_serverAddressLength = sizeof(this->_serverAddress);

    if (connect(this->_serverSocketDescriptor, (sockaddr*)&(this->_serverAddress), (this->_serverAddressLength)) == -1) {
        return(-1);
    }

    this->_connectionStatus = true;

    return(1);
}


ssize_t Socket::RecvDataLen(char *msg, size_t len){
    ssize_t result = 0;
    if (this->_socketRole == Server) {
        result = this->recvall(this->_clientSocketDescriptor, msg, len, 0);
    }
    else if (this->_socketRole == Client) {
        result = this->recvall(this->_serverSocketDescriptor, msg, len, 0);
    }
    else {
        return(-1);
    }
    return(result);
}

ssize_t Socket::SendDataLen(char *msg, size_t len){
    ssize_t result = 0;
    if (this->_socketRole == Server) {
        result = this->sendall(this->_clientSocketDescriptor, msg, len, 0);
    }
    else if (this->_socketRole == Client) {
        result = this->sendall(this->_serverSocketDescriptor, msg, len, 0);
    }
    else {
        return(-1);
    }
    return(result);
}

ssize_t Socket::recvall(int socketDescriptor, char* buf, size_t len, int flags) {
    ssize_t total = 0;
    ssize_t n;
    while (total < len) {
        n = recv(socketDescriptor, buf + total, len - total, flags);
        if (n == -1) { break; }
        total += n;
    }

    if(n==-1){
        this->_connectionStatus = false;
        return -1;
    }else{
        return total;
    }
}

ssize_t Socket::sendall(int socketDescriptor, char* buf, size_t len, int flags) {
    ssize_t total = 0;
    ssize_t n;
    while (total < len) {
        n = send(socketDescriptor, buf + total, len - total, flags);
        if (n == -1) { break; }
        total += n;
    }

    if(n==-1){
        this->_connectionStatus = false;
        return -1;
    }else{
        return total;
    }

}

bool Socket::GetSocketConnectionStatus() {
    return this->_connectionStatus;
}

int& Socket::ReturnDescriptor() {
    return this->_serverSocketDescriptor;
}


Socket::~Socket(){
        if (this->_clientSocketDescriptor != -1) { close(this->_clientSocketDescriptor); }
}

int Socket::CloseConnecton()
{
    if (this->_socketRole==Server && this->_clientSocketDescriptor !=-1){ 
        close(this->_clientSocketDescriptor); 
        return 0;
    }
    else if (this->_socketRole == Client && this->_serverSocketDescriptor != -1) { 
        close(this->_serverSocketDescriptor); 
        return 0;
    }

    return -1;
}
