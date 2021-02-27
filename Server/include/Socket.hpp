#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//ip settings
#define EXTERNAL_SERVER_IP "192.168.0.50" //Server ip for other computers
#define INTERNAL_SERVER_IP "0.0.0.0" //Server ip for server

enum SocketRole {
	UnInicialized = 0,
	Client = 1,
	Server = 2
};


class Socket {
public:

	//Functions for making server socket
	int MakeServerSocket(uint16_t port);
	int Listen();

	//Function for making client socket
	int MakeConnection(uint16_t port);

	//Functions for data transfer 
	int RecvDataLen(char* msg, size_t len);
	int SendDataLen(char* msg, size_t len);

	//Return descriptor for closing Socket
	int& ReturnDescriptor();

	~Socket();

	int CloseConnecton();

	bool GetSocketConnectionStatus();

protected:

	//status (server or client)
	SocketRole _socketRole = UnInicialized;

	//stattus of connection
	bool _connectionStatus = false;

	//socket descriptors
	int _serverSocketDescriptor;
	int _clientSocketDescriptor;

	//connection data
	sockaddr_in _serverAddress;
	sockaddr_in _clientAddress;

	//I don't know what is it, but it needs, chestno
	socklen_t _serverAddressLength = sizeof(this->_serverAddress);
	socklen_t _clientAdressLength = sizeof(this->_clientAddress);

	//Simple functions, for transfer data
	int recvall(int s, char* buf, size_t len, int flags);
	int sendall(int s, char* buf, size_t len, int flags);
};

