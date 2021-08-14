#pragma once

#include <fstream>
#include <string>
#include <cstring>

#include "../../DataStructs/RobotSettingsStruct/RobotSettingsStruct.hpp"
#include "../DataTransmissions/Socket/Socket.hpp"
#include "../BaseProtocol/BaseProtocol.hpp"

namespace DataProtocols {
    class RobotSettingsProtocol : public BaseProtocol{
    private:

        Socket _settingSocket;

        const char *_fileName;

    public:

        explicit RobotSettingsProtocol();
        static RobotSettingsStruct GetSettings();
        void Listen();
    };
}
