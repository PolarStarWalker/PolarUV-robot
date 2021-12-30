#pragma once

#include <fstream>
#include <string>
#include <cstring>

#include "DataTransmissions/Socket/Socket.hpp"

#include "../BaseProtocol/BaseProtocol.hpp"
#include "../../DataStructs/RobotSettingsStruct/RobotSettingsStruct.hpp"


namespace DataProtocols {
    class RobotSettingsProtocol : public BaseProtocol{
    private:
        Socket _settingSocket;

    public:
        explicit RobotSettingsProtocol();

        static RobotSettingsStruct GetSettings();

        [[noreturn]]
        void Start();
        void StartAsync();
    };


}
