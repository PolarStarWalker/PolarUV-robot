#include <fstream>
#include <Exceptions/Exceptions.hpp>

#include "RobotSettings.hpp"
#include "./RobotSettingsMessage.pb.h"

using namespace app;
using Response = lib::network::Response;

inline RobotSettingsData ParseSettings(const RobotSettingsMessage &message) {

    RobotSettingsData settings{};

    settings.HandFreedom = message.hand_coefficient_size();
    auto &hand_coefficient = message.hand_coefficient();
    for (size_t i = 0; i < settings.HandFreedom; ++i)
        settings.HandCoefficientArray[i] = hand_coefficient[i];

    settings.ThrustersNumber = message.thrusters_coefficient_size() / 6;
    auto &thrusters_coefficient = message.thrusters_coefficient();
    for (size_t i = 0; i < settings.ThrustersNumber; ++i)
        settings.ThrustersCoefficientArray.begin()[i] = thrusters_coefficient[i];

    return settings;
}


RobotSettings::RobotSettings(ssize_t id, std::string_view filename) :
        lib::network::IService(id),
        filename_(filename) {
    settings_ = GetSettingsFromDisk();
}

bool RobotSettings::WriteValidate(std::string_view &robotSettings) {

    using InvalidOperation = lib::exceptions::InvalidOperation;

    RobotSettingsMessage message;
    message.ParseFromArray(robotSettings.data(), robotSettings.size());

    if (message.hand_coefficient_size() < 0 || message.hand_coefficient_size() > 4)
        throw InvalidOperation("Неверное количество степеней свободы руки");

    if (message.thrusters_coefficient_size() % 6 != 0)
        throw InvalidOperation("Неправильно задана матрица");

    if (message.thrusters_coefficient_size() < 0 || message.thrusters_coefficient_size() > 8)
        throw InvalidOperation("Неверное количество трастеров");

    if (message.hand_coefficient_size() + message.thrusters_coefficient_size() / 6 > 12)
        throw InvalidOperation("количество двигателей должно быть меньше 12");

    for (auto value: message.thrusters_coefficient())
        if (std::abs(value) > 100)
            throw InvalidOperation("Неверное значение коэффициента");

    for (auto value: message.hand_coefficient())
        if (std::abs(value) > 100)
            throw InvalidOperation("Неверное значение коэффициента");

    return true;
}

Response RobotSettings::Write(std::string_view &robotSettings) {

    {
        std::fstream file(filename_.data(), std::ios::trunc | std::ios::out | std::ios::binary);
        file.write(robotSettings.data(), robotSettings.size());
    }

    RobotSettingsMessage message;
    message.ParseFromArray(robotSettings.data(), robotSettings.size());
    settings_ = ParseSettings(message);

    return {std::string(), lib::network::Response::NoContent, serviceId_};
}

Response RobotSettings::Read(std::string_view &request) {
    std::fstream file(filename_.data(), std::ios::in | std::ios::binary);

    file.seekg(0, std::fstream::end);
    ssize_t length = file.tellg();
    file.seekg(0, std::fstream::beg);

    std::string out(length, 0);

    file.read(out.data(), length);

    return {std::move(out), lib::network::Response::Ok, serviceId_};
}

RobotSettingsData RobotSettings::GetSettingsFromDisk() {
    RobotSettingsMessage message;

    {
        std::fstream file(filename_.data(), std::ios::in | std::ios::binary);
        message.ParseFromIstream(&file);
    }

    return ParseSettings(message);
}


