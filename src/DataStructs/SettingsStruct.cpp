#include "./SettingStruct/SettingsStruct.hpp"

SettingsStruct::SettingsStruct() {
    this->HandFreedom = -1;
    this->HandCoefficientArray = nullptr;
    this->ThrustersNumber = -1;
    this->MoveCoefficientArray = nullptr;
    this->MaxMotorSpeed = -1;
    this->MotorsProtocol = -1;
}

SettingsStruct::SettingsStruct(SettingsStruct &&settingsStruct) {
    this->HandFreedom = settingsStruct.HandFreedom;
    this->ThrustersNumber = settingsStruct.ThrustersNumber;
    this->MotorsProtocol = settingsStruct.MotorsProtocol;
    this->MaxMotorSpeed = settingsStruct.MaxMotorSpeed;

    this->MoveCoefficientArray = settingsStruct.MoveCoefficientArray;
    settingsStruct.MoveCoefficientArray = nullptr;

    this->HandCoefficientArray = settingsStruct.HandCoefficientArray;
    settingsStruct.HandCoefficientArray = nullptr;
}

SettingsStruct::SettingsStruct(const SettingsStruct &settingsStruct) {
    this->HandFreedom = settingsStruct.HandFreedom;
    this->ThrustersNumber = settingsStruct.ThrustersNumber;
    this->MotorsProtocol = settingsStruct.MotorsProtocol;
    this->MaxMotorSpeed = settingsStruct.MaxMotorSpeed;

    this->MoveCoefficientArray = new double [6 * this->ThrustersNumber];
    for (size_t i = 0; i < 6 * this->ThrustersNumber; i++) {
        this->MoveCoefficientArray[i] = settingsStruct.MoveCoefficientArray[i];
    }

    this->HandCoefficientArray = new double[this->HandFreedom];
    for(size_t i = 0; i< this->HandFreedom; i++){
        this->HandCoefficientArray[i] = settingsStruct.HandCoefficientArray[i];
    }
}

SettingsStruct::~SettingsStruct() {
    delete[] this->MoveCoefficientArray;
    delete[] this->HandCoefficientArray;
}

void SettingsStruct::SetMoveCoefficientArray(const std::vector<double> &copyMoveArray) {
    delete MoveCoefficientArray;

    this->ThrustersNumber = copyMoveArray.size() / 6;
    this->MoveCoefficientArray = new double[copyMoveArray.size()];

    for (size_t i = 0; i < this->ThrustersNumber; i++) {
        for (size_t j = 0; j < 6; j++) {
            this->MoveCoefficientArray[i * 6 + j] = copyMoveArray[i * 6 + j];
        }
    };
}

void SettingsStruct::SetHandCoefficientArray(const std::vector<double> &copyHandArray) {
    delete HandCoefficientArray;

    this->HandFreedom = copyHandArray.size();
    this->HandCoefficientArray = new double[this->HandFreedom];
    for (size_t i = 0; i < this->HandFreedom; i++) {
        this->HandCoefficientArray[i] = copyHandArray[i];
    }
}

std::ostream &operator<<(std::ostream &ostream, const SettingsStruct &settingStruct) {

    ostream << "ThrustersNumber: " << (int32_t) settingStruct.ThrustersNumber << std::endl;

    ostream << "MoveCoefficientMatrix:" << std::endl;
    for (size_t i = 0; i < settingStruct.ThrustersNumber; i++) {
        ostream << "[ ";
        for (size_t j = 0; j < 5; j++) {
            ostream << settingStruct.MoveCoefficientArray[6 * i + j] << ", ";
        }
        ostream << settingStruct.MoveCoefficientArray[6 * i + 5] << "]" << std::endl;
    }

    ostream << "HandFreedom: " << (ssize_t) settingStruct.HandFreedom << std::endl;
    ostream << "HandCoefficientArray: [";
    for (size_t i = 0; i < settingStruct.HandFreedom - 1; i++) {
        ostream << settingStruct.HandCoefficientArray[i] << ", ";
    }
    ostream << settingStruct.HandCoefficientArray[settingStruct.HandFreedom - 1] << "]" << std::endl;

    ostream << "MotorProtocol: ";
    switch (settingStruct.MotorsProtocol) {
        case 1:
            std::cout << "DShot150";
            break;
        case 2:
            std::cout << "DShot300";
            break;
        case 4:
            std::cout << "DShot600";
            break;
        case 8:
            std::cout << "DShot1200";
            break;
    }
    ostream << std::endl;

    ostream << "MaxMotorSpeed: " << settingStruct.MaxMotorSpeed << std::endl;

    return ostream;
}

const double *SettingsStruct::MoveCoefficientArrayBegin() const {
    return this->MoveCoefficientArray;
}

int8_t SettingsStruct::ThrusterNumber() const {
    return this->ThrustersNumber;
}

const double *SettingsStruct::HandCoefficientArrayBegin() const {
    return this->MoveCoefficientArray;
}

int8_t SettingsStruct::HandFreedoms() const {
    return this->ThrustersNumber;
}

