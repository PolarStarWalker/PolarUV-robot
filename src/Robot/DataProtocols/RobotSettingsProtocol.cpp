#include "RobotSettingsProtocol/RobotSettingsProtocol.hpp"


using namespace DataProtocols;

RobotSettingsProtocol::RobotSettingsProtocol() : _settingSocket() {
    this->_settingSocket.MakeServerSocket(14322);
}

void RobotSettingsProtocol::StartAsync() {
    std::thread thread(&RobotSettingsProtocol::Start, this);
    thread.detach();
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

            BaseRobotSettingsStruct robotStatic{};

            _settingSocket.RecvDataLen((char *) &robotStatic, BaseRobotSettingsStructActualSize);

            RobotSettingsStruct robotSettingsStruct(robotStatic);

            _settingSocket.RecvDataLen((char *) robotSettingsStruct.Begin() + BaseRobotSettingsStructActualSize,
                                       robotSettingsStruct.Size() - BaseRobotSettingsStructActualSize);

            std::fstream file("robotSettings", std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);

            file.write(robotSettingsStruct.Begin(), robotSettingsStruct.Size());

            std::cout << robotSettingsStruct;

            continue;
        }
    }
}

RobotSettingsStruct RobotSettingsProtocol::GetSettings() {

    ///ToDo: сделать валидацию для чтения
    std::fstream file("robotSettings", std::ios_base::binary | std::ios_base::in);

    BaseRobotSettingsStruct robotStatic{};

    file.read((char *) &robotStatic, BaseRobotSettingsStructActualSize);

    RobotSettingsStruct robotSettingsStruct(robotStatic);

    file.read(robotSettingsStruct.Begin() + BaseRobotSettingsStructAllocatedSize,
              robotSettingsStruct.Size() - BaseRobotSettingsStructAllocatedSize);

    return robotSettingsStruct;
}
