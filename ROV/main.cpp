#pragma once

#include <iostream>

#include "DataStruct.hpp"
#include "Socket.hpp"

#include "IntMatrixClass.hpp"
#include "FloatMatrixClass.hpp"


using namespace std;

int main(void) {

    CommandsStruct* commandsStruct = new CommandsStruct;
    MotorsStruct* motorsStruct = new MotorsStruct;


    char motorsMessage[MotorsStructLenMessage] = {};


    Socket socket;
    socket.MakeServerSocket(1999);
    


    for (;;) 
    {
        while (socket.GetSocketConnectionStatus()) {
            cout << "Hello World\n";
        }

        socket.Listen();
    }

    delete commandsStruct;
    delete motorsStruct;

    return 0;
}

