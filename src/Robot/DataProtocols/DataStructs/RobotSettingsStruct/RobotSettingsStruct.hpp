#ifndef ROBOT_ROBOTSETTINGSSTRUCT_HPP
#define ROBOT_ROBOTSETTINGSSTRUCT_HPP

#include <iostream>
#include <cstring>
#include <cmath>

namespace {
    enum Offset : size_t {
        MotorsProtocolOffset = 0,
        MaxMotorSpeedOffset = 2,
        ThrusterNumberOffset = 4,
        HandFreedomOffset = 6,
        ArraysOffset = 8
    };
}

struct BaseRobotSettingsStruct{
    int16_t MotorsProtocol;
    int16_t MaxMotorSpeed;
    int16_t ThrusterNumber;
    int16_t HandFreedom;
};

constexpr size_t BaseRobotSettingsStructLen = sizeof(BaseRobotSettingsStruct);

class RobotSettingsStruct {
public:

    RobotSettingsStruct();
    //ToDo сделать когда-нибудь более типизированный класс
    explicit RobotSettingsStruct(const BaseRobotSettingsStruct& robotStatic);
    explicit RobotSettingsStruct(size_t thrustersNumber, size_t handFreedom);
    RobotSettingsStruct(const RobotSettingsStruct &robotSettingsStruct);
    RobotSettingsStruct(RobotSettingsStruct &&robotSettingsStruct) noexcept ;

    ~RobotSettingsStruct();

    RobotSettingsStruct &operator=(const RobotSettingsStruct &robotSettingsStruct) noexcept;
    RobotSettingsStruct &operator=(RobotSettingsStruct &&robotSettingsStruct) noexcept;

    char *Begin();
    char *End();

    size_t Size();

    double *const ThrusterCoefficientArray();
    int16_t ThrusterNumber();

    double *const HandCoefficientArray();
    int16_t HandFreedom();

    int16_t &MaxMotorsSpeed();
    int16_t &MotorsProtocol();

private:
    char *_data;
    size_t _handArrayOffset;
    size_t _length;

    friend std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingsStruct);
};

std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingsStruct);

#endif
