#ifndef ROBOT_SETTINGSSTRUCT_HPP
#define ROBOT_SETTINGSSTRUCT_HPP
#include <iostream>

///ToDo: переделать в класс с функциями для изменения массива, а то вот так динамическая память может утечь
/// А ещё move semantics

struct SettingsStruct {
    double *MoveCoefficientArray = nullptr;
    double *HandCoefficientArray = nullptr;
    ssize_t MaxMotorSpeed = -1;
    int8_t ThrustersNumber = -1;
    int8_t MotorsProtocol = -1;
    int8_t HandFreedom = -1;
    bool IsTurnOn = false;
};

extern SettingsStruct SettingsStructData;
constexpr size_t SettingsStructLen = sizeof(SettingsStructData);

std::ostream& operator << (std::ostream &ostream, const SettingsStruct &settingStruct);

#endif
