#include "RobotSettingsProtocol/RobotSettingsProtocol.hpp"


using namespace DataProtocols;

RobotSettingsProtocol::RobotSettingsProtocol() : _settingSocket() {
    this->_settingSocket.MakeServerSocket(14322);
}

void RobotSettingsProtocol::Start() {
    for (;;) {

        this->_settingSocket.Listen();
        char direction = 0;
        _settingSocket.RecvDataLen(&direction, sizeof(direction));

        if (direction == 'r') {

            RobotSettingsStruct robotSettingsStruct = GetSettings();

            ssize_t size = _settingSocket.SendDataLen(robotSettingsStruct.Begin(), robotSettingsStruct.Size());

            std::cout << size << std::endl;
            std::cout << robotSettingsStruct;

            continue;
        }

        if (direction == 'w') {

            int16_t robotStatic[4]{};

            _settingSocket.RecvDataLen((char *) &robotStatic, ArraysOffset);

            RobotSettingsStruct robotSettingsStruct(robotStatic[2], robotStatic[3]);
            robotSettingsStruct.MaxMotorsSpeed() = robotStatic[1];
            robotSettingsStruct.MotorsProtocol() = robotStatic[0];

            _settingSocket.RecvDataLen((char *) robotSettingsStruct.Begin() + ArraysOffset,
                                       robotSettingsStruct.Size() - ArraysOffset);

            std::fstream file("robotSettings", std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);

            file.write(robotSettingsStruct.Begin(), robotSettingsStruct.Size());

            continue;
        }
    }
}

void RobotSettingsProtocol::StartAsync() {
    std::thread thread(&RobotSettingsProtocol::Start, this);
    thread.detach();
}


RobotSettingsStruct RobotSettingsProtocol::GetSettings() {

    ///ToDo: сделать валидацию для чтения
    std::fstream file("robotSettings", std::ios_base::binary | std::ios_base::in);

    int16_t robotStatic[4]{};

    file.read((char *) robotStatic, 8);

    if(robotStatic[2] == 0 ||  robotStatic[3] == 0){
        return RobotSettingsStruct();
    }

    RobotSettingsStruct robotSettingsStruct(robotStatic[2], robotStatic[3]);

    robotSettingsStruct.MotorsProtocol() = robotStatic[0];
    robotSettingsStruct.MaxMotorsSpeed() = robotStatic[1];


    file.read(robotSettingsStruct.Begin() + ArraysOffset, robotSettingsStruct.Size() - ArraysOffset);

    return robotSettingsStruct;
}
