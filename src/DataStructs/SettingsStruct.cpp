#include "./SettingStruct/SettingsStruct.hpp"

std::ostream &operator<<(std::ostream &ostream, const SettingsStruct &settingStruct) {

    ostream << "ThrusterNumber: " << settingStruct.ThrustersNumber << std::endl;

    ostream << "MoveCoefficientMatrix:" << std::endl;
    for (size_t i = 0; i < settingStruct.ThrustersNumber; i++) {
        ostream << "[ ";
        for (size_t j = 0; j < 5; j++) {
            ostream << settingStruct.MoveCoefficientArray[6 * i + j] << ", ";
        }
        ostream << settingStruct.MoveCoefficientArray[6 * i + 6] << "]" << std::endl;
    }

    ostream << "HandFreedom: " << (ssize_t) settingStruct.HandFreedom << std::endl;
    ostream << "HandCoefficientArray: [";
    for (size_t i = 0; i < settingStruct.HandFreedom - 1; i++) {
        ostream << settingStruct.HandCoefficientArray[i] << ", ";
    }
    ostream << settingStruct.HandCoefficientArray[settingStruct.HandFreedom - 1] << ", ]" << std::endl;

    std::cout << "MotorProtocol: " << (long) settingStruct.MotorsProtocol << std::endl;
    std::cout << "MaxMotorSpeed: " << settingStruct.MaxMotorSpeed << std::endl;

    return ostream;
}