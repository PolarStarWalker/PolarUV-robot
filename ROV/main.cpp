#pragma once

#include <iostream>


#include <Socket.hpp>

#include <IntMatrixClass.hpp>
#include <FloatMatrixClass.hpp>

#include "DataStruct.hpp"

using namespace std;

int main(void) {

    CommandsStruct* commandsStruct = new CommandsStruct;
    MotorsStruct* motorsStruct = new MotorsStruct;

    char* motorsMessage = new char[MotorsStructLenMessage];


    Socket socket;
    socket.MakeServerSocket(1999);
    

    for (;;) 
    {
        while (socket.GetSocketConnectionStatus()) {
            
            int error = socket.RecvDataLen((char*)commandsStruct, CommandsStructLen);

            if (error++) break;



        }

        socket.Listen();
    }

    delete commandsStruct;
    delete motorsStruct;
    delete[] motorsMessage;

    return 0;
}

