#ifndef ROBOT_SETTINGSSTRUCT_HPP
#define ROBOT_SETTINGSSTRUCT_HPP

#include <iostream>
#include <vector>

///ToDo: переделать в класс с функциями для изменения массива, а то вот так динамическая память может утечь
/// А ещё move semantics
class SettingsProtocol;

class SettingsStruct {
public:
    SettingsStruct();
    SettingsStruct(SettingsStruct &&settingsStruct) noexcept ;
    SettingsStruct(const SettingsStruct &settingsStruct);
    ~SettingsStruct();

    const double* MoveCoefficientArrayBegin() const;
    int8_t ThrusterNumber() const;
    const double* HandCoefficientArrayBegin() const;
    int8_t HandFreedoms() const;

    void SetMoveCoefficientArray(const std::vector<double> &copyMoveArray);
    void SetHandCoefficientArray(const std::vector<double> &copyHandArray);

private:
    double* MoveCoefficientArray = nullptr;
    double* HandCoefficientArray = nullptr;
public:
    int32_t MaxMotorSpeed = -1;
private:
    int8_t ThrustersNumber = -1;
    int8_t HandFreedom = -1;
public:
    int8_t MotorsProtocol = -1;

    friend std::ostream &operator<<(std::ostream &ostream, const SettingsStruct &settingStruct);
    friend SettingsProtocol;
};

extern SettingsStruct SettingsStructData;
constexpr size_t SettingsStructLen = sizeof(SettingsStructData);

std::ostream &operator<<(std::ostream &ostream, const SettingsStruct &settingStruct);


#endif
