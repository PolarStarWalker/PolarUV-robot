#include "main.hpp"

int main() {
    ///program setting
    CommandsStruct* commandsStruct = new CommandsStruct;
    MotorsStruct* motorsStruct = new MotorsStruct;

    char* motorsMessage = new char[MotorsStructLenMessage];

    SettingsStruct settingsStruct;
    SettingsFileService settingsFileService("settings");
    settingsFileService.GetSettings(&settingsStruct);

    Socket socket;
    socket.MakeServerSocket(1999);

    /// test
    IntMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    FloatMatrixClass vectorsMatrix(6);

    vectorsMatrix = commandsStruct->VectorArray;
    coefficientMatrix = settingsStruct.CoefficientArray;



    std::cout<<"IsTurnOn: "<<settingsStruct.IsTurnOn<<std::endl;
    std::cout<<"ThrusterNumber: "<<settingsStruct.ThrustersNumber<<std::endl;
    std::cout<<"CoefficientArray: \n";
    for (size_t m = 0; m < coefficientMatrix.GetRows(); m++)
    {
        for (size_t n = 0; n < coefficientMatrix.GetColumns(); n++)
        {
            std::cout << coefficientMatrix[m][n] << " ";
        }
        std::cout << "\n";
    }

    std::cout<<"MaxMotorSpeed: "<<settingsStruct.MaxMotorSpeed<<std::endl;
    std::cout<<"MotorProtocol: "<<settingsStruct.MotorsProtocol<<std::endl;

    /// main program
    while (false)
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