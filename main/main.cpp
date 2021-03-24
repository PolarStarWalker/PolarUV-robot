#include "main.hpp"

int main(void) {
    ///program setting
    CommandsStruct* commandsStruct = new CommandsStruct;
    MotorsStruct* motorsStruct = new MotorsStruct;

    char* motorsMessage = new char[MotorsStructLenMessage];

    SettingsStruct settingsStruct;

    Socket socket;
    socket.MakeServerSocket(1999);

    /// test
    IntMatrixClass coefficientMatrix(8, 6);
    FloatMatrixClass vectorsMatrix(6);

    int64_t** matrix = new int64_t * [8]{};
    for (size_t i = 0; i < 8; i++) { matrix[i] = new int64_t[6]{}; }


    SettingsFileService file("settings");
    file.GetSettings(&settingsStruct);

    vectorsMatrix = commandsStruct->VectorArray;
    coefficientMatrix = matrix;


    for (size_t m = 0; m < 8; m++)
    {
        for (size_t n = 0; n < 6; n++)
        {
            std::cout << matrix[m][n] << " ";
        }
        std::cout << "\n";
    }


    /// main program
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