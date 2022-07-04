#include <fstream>
#include <Exceptions/Exceptions.hpp>

#include "RobotSettings.hpp"
#include "./RobotSettingsMessage.pb.h"

using namespace app;
using Response = lib::network::Response;
using ResponseBufferType = lib::network::IService::ResponseBufferType;

inline RobotSettingsStruct ParseSettings(const RobotSettingsMessage &message) {

    RobotSettingsStruct settings{};

    settings.HandFreedom = message.hand_coefficient_size();
    const auto &hand_coefficient = message.hand_coefficient();
    for (size_t i = 0; i < settings.HandFreedom; ++i)
        settings.HandCoefficientArray[i] = hand_coefficient[i];

    settings.ThrustersNumber = message.thrusters_coefficient_size() / 6;
    const auto &thrusters_coefficient = message.thrusters_coefficient();
    for (int i = 0; i < message.thrusters_coefficient_size(); ++i) {
        auto value = thrusters_coefficient[i];
        settings.ThrustersCoefficientArray.begin()->begin()[i] = value;
    }

    for(auto i : std::ranges::iota_view(0,4)) {
        settings.PIDCoefficients.PArray[i] = message.pid()[i].p();
        settings.PIDCoefficients.IArray[i] = message.pid()[i].i();
        settings.PIDCoefficients.DArray[i] = message.pid()[i].d();
    }

    return settings;
}

RobotSettingsStruct GetSettingsFromDisk(std::string_view filename) {

    RobotSettingsMessage message;
    RobotSettingsStruct settings;

    std::fstream file(filename.data(), std::ios::in | std::ios::binary);
    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > 0) {
        message.ParseFromIstream(&file);
        settings = ParseSettings(message);
    }

    return settings;
}


RobotSettings::RobotSettings(ssize_t id, std::string_view filename) :
        lib::network::IService(id),
        filename_(filename),
        settings_() {
    SetSettings(GetSettingsFromDisk(filename_));
    std::cout << settings_ << std::endl;
}

bool RobotSettings::WriteValidate(const std::string_view &robotSettings) {

    using InvalidOperation = lib::exceptions::InvalidOperation;

    RobotSettingsMessage message;
    message.ParseFromArray(robotSettings.data(), robotSettings.size());

    if (message.hand_coefficient_size() < 0 || message.hand_coefficient_size() > 4)
        throw InvalidOperation("Неверное количество степеней свободы руки");

    if (message.thrusters_coefficient_size() % 6 != 0)
        throw InvalidOperation("Неправильно задана матрица");

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

void RobotSettings::Write(const std::string_view &robotSettings) {

    {
        std::fstream file(filename_.data(), std::ios::trunc | std::ios::out | std::ios::binary);
        file.write(robotSettings.data(), robotSettings.size());
    }

    RobotSettingsMessage message;
    message.ParseFromArray(robotSettings.data(), robotSettings.size());
    SetSettings(ParseSettings(message)) ;

    //   std::cout << settings_ << std::endl;
}

ResponseBufferType RobotSettings::Read() {
    std::fstream file(filename_.data(), std::ios::in | std::ios::binary);

    file.seekg(0, std::fstream::end);
    auto length = file.tellg();
    file.seekg(0, std::fstream::beg);

    std::string out;

    if(length > 0){
        out.resize(length);
        file.read(out.data(), length);
    } else {
        RobotSettingsMessage message;

        for(size_t i = 0; i < 6 * 8; ++i)
            message.add_thrusters_coefficient(0);

        message.add_hand_coefficient(0);

        message.set_motors_protocol(RobotSettingsMessage::DSHOT150);
        message.set_maximum_motor_speed(4000);

        out = message.SerializeAsString();
    }

    return out;
}

inline void RobotSettings::SetSettings(const RobotSettingsStruct& settingsData) {
    std::lock_guard lock(settingsMutex_);
    settings_ = settingsData;
}


RobotSettingsStruct RobotSettings::GetSettings() const {
    std::unique_lock lock(settingsMutex_);
    return settings_;
}

