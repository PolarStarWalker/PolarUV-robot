#pragma once
#include <iostream>
#include <atomic>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>


//ip settings
#define INTERNAL_SERVER_IP "0.0.0.0" //Server ip for server

class Socket {
public:

    explicit Socket(uint16_t port);
    ~Socket();

	int Listen();

	//Functions for data transfer
	ssize_t RecvDataLen(char* msg, size_t len);
	ssize_t SendDataLen(char* msg, size_t len);

	bool IsOnline() const;

private:
	//socket descriptors
	int _serverSocketDescriptor;
	int _clientSocketDescriptor;

	//connection data
	sockaddr_in _serverAddress;
	sockaddr_in _clientAddress;

	//I don't know what is it, but it needs, chestno
	socklen_t _serverAddressLength = sizeof(this->_serverAddress);
	socklen_t _clientAdressLength = sizeof(this->_clientAddress);

    //status of connection
    std::atomic<bool> _isOnline = false;

	//Simple function, for transfer data
	ssize_t recvall(int s, char* buf, size_t len, int flags);
	ssize_t sendall(int s, char* buf, size_t len, int flags);
};

