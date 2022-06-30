#ifndef ROBOT_STARTSETTINGS_HPP
#define ROBOT_STARTSETTINGS_HPP

#include "../MotorsSender/IMotorsSender.hpp"
#include "../MotorsSender/SPI.hpp"


//ToDo: потом отрефакторить шоб всё было по красоте
class StartSettings {
public:

    enum Mode : uint8_t {
        AUV,
        ROV
    };

    explicit StartSettings(const std::string_view &path);

    ~StartSettings();

    MotorsSender::IMotorsSender &GetMotorsSender();

    [[nodiscard]] Mode GetMode();

    [[nodiscard]] std::string_view GetSensorsPath();

private:
    Mode mode_;
    std::string sensorsPath_;
    MotorsSender::IMotorsSender *motorsSender_;
};


#endif
