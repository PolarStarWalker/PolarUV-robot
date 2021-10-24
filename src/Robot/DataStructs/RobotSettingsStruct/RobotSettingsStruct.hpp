#ifndef ROBOT_ROBOTSETTINGSSTRUCT_HPP
#define ROBOT_ROBOTSETTINGSSTRUCT_HPP

#include <iostream>
#include <cstring>
#include <cmath>

struct BaseRobotSettingsStruct {
    uint16_t MotorsProtocol;
    uint16_t MaxMotorSpeed;
    uint16_t ThrusterNumber;
    uint16_t HandFreedom;
};
///Find real size of BaseRobotSettingsStruct
constexpr size_t BaseRobotSettingsStructActualSize = sizeof(BaseRobotSettingsStruct);
///Find how many bytes to allocate given eight byte alignment
const size_t BaseRobotSettingsStructAllocatedSize =
        (std::ceil(((double) sizeof(BaseRobotSettingsStruct)) / sizeof(double))) * sizeof(double);

class RobotSettingsStruct {
public:

    RobotSettingsStruct();

    //ToDo сделать когда-нибудь более типизированный класс
    explicit RobotSettingsStruct(const BaseRobotSettingsStruct &robotStatic);

    RobotSettingsStruct(const RobotSettingsStruct &robotSettingsStruct);

    RobotSettingsStruct(RobotSettingsStruct &&robotSettingsStruct) noexcept;

    ~RobotSettingsStruct();

    RobotSettingsStruct &operator=(const RobotSettingsStruct &robotSettingsStruct) noexcept;

    RobotSettingsStruct &operator=(RobotSettingsStruct &&robotSettingsStruct) noexcept;

    char *Begin();

    char *End();

    size_t Size();

    float *ThrusterCoefficientArray();

    uint16_t ThrusterNumber();

    float *HandCoefficientArray();

    uint16_t HandFreedom();

    uint16_t &MaxMotorsSpeed();

    uint16_t &MotorsProtocol();

private:
    char *_data;
    size_t _size;

    float *_handCoefficientArrayPtr;
    float *_thrusterCoefficientArrayPtr;

    friend std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingsStruct);
};

std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingsStruct);

#endif
