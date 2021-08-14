#ifndef ROBOT_ROBOTSETTINGSSTRUCT_HPP
#define ROBOT_ROBOTSETTINGSSTRUCT_HPP

#include <iostream>
#include <vector>
#include <cstring>


namespace {
    enum Offset : size_t {
        MotorsProtocolOffset = 0,
        MaxMotorSpeedOffset = 2,
        ThrusterNumberOffset = 4,
        HandFreedomOffset = 6,
        ArraysOffset = 8
    };
}

struct RobotSettingsStructStatic{
    uint16_t MotorsProtocol;
    uint16_t MaxMotorSpeed;
    uint16_t ThrusterNumber;
    uint16_t HandFreedom;
};

class RobotSettingsStruct {
public:

    RobotSettingsStruct();
    explicit RobotSettingsStruct(const RobotSettingsStructStatic& robotStatic);
    RobotSettingsStruct(const std::vector<double> &copyMoveArray, const std::vector<double> &copyHandArray);

    explicit RobotSettingsStruct(size_t thrustersNumber, size_t handFreedom);

    RobotSettingsStruct(RobotSettingsStruct &&robotSettingsStruct);

    RobotSettingsStruct(const RobotSettingsStruct &robotSettingsStruct);

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
