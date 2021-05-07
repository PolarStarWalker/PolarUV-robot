#include "main.hpp"
#include <array>
#include <sched.h>

int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///Set program
    ///structs for transfer data
    CommandsStruct commandsStruct;
    MotorsStruct motorsStruct;

    ///bufer
    char *motorsMessage[2 * MotorsStructLenMessage]{};
    ///settings from settings file
    SettingsFileService settingsFileService("settings");
    SettingsStruct settingsStruct = settingsFileService.GetSettings();

    ///Set data protocols
    Socket socket;
    socket.MakeServerSocket(1999);
    SPI commandSender("/dev/spidev0.0", 35000000);
    UART uart("/dev/ttyS4", S115200);

    /// program objects
    FloatVectorClass moveVector(6);
    FloatMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    coefficientMatrix = settingsStruct.MoveCoefficientArray;
    coefficientMatrix *= -settingsStruct.MaxCommandValue;

    /////// ---------- Проверка ----------
    uint8_t low = 0x00, high = 0xFF;
    while (1){
        for (int i = 0; i < 9999; i++){
            uart.send(&high,1);
        }
        for (int i = 0; i < 9999; i++){
            uart.send(&low,1);
        }
    }
    /////// ------------------------------

    return 0;
}
