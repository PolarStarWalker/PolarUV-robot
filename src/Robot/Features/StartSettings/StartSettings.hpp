#ifndef ROBOT_STARTSETTINGS_HPP
#define ROBOT_STARTSETTINGS_HPP

#include <fstream>
#include "../MotorsSender/IMotorsSender.hpp"
#include "../MotorsSender/SPI.hpp"

#include "Math/SIPrefix.hpp"

class StartSettings {
public:

    StartSettings() :
            motorsSender_(nullptr),
            motorsSenderId_(MotorsSender::SPI){
        std::fstream file("StartSettings", std::ios_base::in | std::ios_base::binary);
        file.read((char *) this, sizeof(StartSettings));
    }

    ~StartSettings(){
        delete motorsSender_;
    };

    MotorsSender::IMotorsSender &GetMotorsSender() {
        switch (motorsSenderId_) {
            case MotorsSender::SPI:
                motorsSender_ = new class MotorsSender::SPI("/dev/spidev0.0", Mega(6));
                return *motorsSender_;
        }
        std::terminate();
    }


private:
    MotorsSender::IMotorsSender *motorsSender_;
    MotorsSender::Id motorsSenderId_;

};


#endif
