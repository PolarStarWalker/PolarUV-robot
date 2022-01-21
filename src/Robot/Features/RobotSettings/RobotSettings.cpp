#include <fstream>
#include <Exceptions/Exceptions.hpp>

#include "RobotSettings.hpp"
#include "./RobotSettingsMessage.pb.h"

using namespace app;

RobotSettings::RobotSettings(ssize_t id, std::string_view filename) :
        lib::network::IService(id),
        filename_(filename) {}

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

    for(auto value : message.thrusters_coefficient())
        if(std::abs(value) > 100)
            throw InvalidOperation("Неверное значение коэффициента");

    for(auto value : message.hand_coefficient())
        if(std::abs(value) > 100)
            throw InvalidOperation("Неверное значение коэффициента");

    return true;
}

lib::network::Response RobotSettings::Write(std::string_view &robotSettings) {

    WriteValidate(robotSettings);

    std::fstream file(filename_.data(), std::ios::trunc | std::ios::out | std::ios::binary);
    file.write(robotSettings.data(), robotSettings.size());

    return {std::string(), lib::network::Response::NoContent, serviceId_};
}

lib::network::Response RobotSettings::Read(std::string_view &request) {
    std::fstream file(filename_.data(), std::ios::in | std::ios::binary);

    file.seekg(0, std::fstream::end);
    ssize_t length = file.tellg();
    file.seekg(0, std::fstream::beg);

    std::string out(length, 0);

    file.read(out.data(), length);

    return {std::move(out), lib::network::Response::Ok, serviceId_};
}


