#include "./RobotSettingsStruct/RobotSettingsStruct.hpp"

RobotSettingsStruct::RobotSettingsStruct() {
    this->_size = BaseRobotSettingsStructAllocatedSize + 7 * sizeof(double);

    this->_data = new char[this->_size]{};

    ((BaseRobotSettingsStruct *) this->_data)->MotorsProtocol = 1;
    ((BaseRobotSettingsStruct *) this->_data)->MaxMotorSpeed = 1000;
    ((BaseRobotSettingsStruct *) this->_data)->ThrusterNumber = 1;
    ((BaseRobotSettingsStruct *) this->_data)->HandFreedom = 1;

    this->_thrusterCoefficientArrayPtr = (double *) (this->_data + BaseRobotSettingsStructAllocatedSize);
    this->_handCoefficientArrayPtr = this->_thrusterCoefficientArrayPtr + 6;
}

RobotSettingsStruct::RobotSettingsStruct(const BaseRobotSettingsStruct &robotStatic) {
    size_t handFreedom = robotStatic.HandFreedom == 0 ? 1 : robotStatic.HandFreedom;
    size_t thrusterNumber = robotStatic.ThrusterNumber == 0 ? 1 : robotStatic.ThrusterNumber;

    this->_size = BaseRobotSettingsStructAllocatedSize +
                  thrusterNumber * 6 * sizeof(double) +
                  handFreedom * sizeof(double);

    this->_data = new char[this->_size]{};

    ((BaseRobotSettingsStruct *) this->_data)->MotorsProtocol = robotStatic.MotorsProtocol;
    ((BaseRobotSettingsStruct *) this->_data)->MaxMotorSpeed = robotStatic.MaxMotorSpeed;
    ((BaseRobotSettingsStruct *) this->_data)->ThrusterNumber = thrusterNumber;
    ((BaseRobotSettingsStruct *) this->_data)->HandFreedom = handFreedom;

    this->_thrusterCoefficientArrayPtr = (double *) (this->_data + BaseRobotSettingsStructAllocatedSize);
    this->_handCoefficientArrayPtr = _thrusterCoefficientArrayPtr + 6 * robotStatic.ThrusterNumber;
}

RobotSettingsStruct::RobotSettingsStruct(const RobotSettingsStruct &robotSettingsStruct) {
    this->_size = robotSettingsStruct._size;
    this->_data = new char[this->_size];

    for (size_t i = 0; i < this->_size / 8; i++)
        ((uint64_t *) this->_data)[i] = ((uint64_t *) robotSettingsStruct._data)[i];

    size_t deltaThrusterCoefficientArray =
            robotSettingsStruct._thrusterCoefficientArrayPtr - (double *) robotSettingsStruct._data;
    this->_thrusterCoefficientArrayPtr = ((double *) this->_data) + deltaThrusterCoefficientArray;

    size_t deltaHandCoefficientArray =
            robotSettingsStruct._thrusterCoefficientArrayPtr - (double *) robotSettingsStruct._data;

    this->_thrusterCoefficientArrayPtr = _thrusterCoefficientArrayPtr + deltaHandCoefficientArray;
}

RobotSettingsStruct::RobotSettingsStruct(RobotSettingsStruct &&robotSettingsStruct) noexcept {
    this->_size = robotSettingsStruct._size;
    this->_data = robotSettingsStruct._data;
    this->_thrusterCoefficientArrayPtr = robotSettingsStruct._thrusterCoefficientArrayPtr;
    this->_handCoefficientArrayPtr = robotSettingsStruct._handCoefficientArrayPtr;
    robotSettingsStruct._data = nullptr;
}


RobotSettingsStruct::~RobotSettingsStruct() {
    delete[] this->_data;
}

RobotSettingsStruct &RobotSettingsStruct::operator=(const RobotSettingsStruct &robotSettingsStruct) noexcept {
    delete[] this->_data;

    this->_size = robotSettingsStruct._size;
    this->_data = new char[this->_size];

    for (size_t i = 0; i < this->_size / 8; i++)
        ((uint64_t *) this->_data)[i] = ((uint64_t *) robotSettingsStruct._data)[i];

    size_t deltaThrusterCoefficientArray =
            robotSettingsStruct._thrusterCoefficientArrayPtr - (double *) robotSettingsStruct._data;
    this->_thrusterCoefficientArrayPtr = ((double *) this->_data) + deltaThrusterCoefficientArray;

    size_t deltaHandCoefficientArray =
            robotSettingsStruct._thrusterCoefficientArrayPtr - (double *) robotSettingsStruct._data;

    this->_thrusterCoefficientArrayPtr = _thrusterCoefficientArrayPtr + deltaHandCoefficientArray;

    return *this;
}

RobotSettingsStruct &RobotSettingsStruct::operator=(RobotSettingsStruct &&robotSettingsStruct) noexcept {
    delete[] this->_data;

    this->_size = robotSettingsStruct._size;
    this->_data = robotSettingsStruct._data;
    this->_thrusterCoefficientArrayPtr = robotSettingsStruct._thrusterCoefficientArrayPtr;
    this->_handCoefficientArrayPtr = robotSettingsStruct._handCoefficientArrayPtr;
    robotSettingsStruct._data = nullptr;

    return *this;
}


char *RobotSettingsStruct::Begin() {
    return this->_data;
}

char *RobotSettingsStruct::End() {
    return this->_data + this->_size;
}

size_t RobotSettingsStruct::Size() {
    return this->_size;
}

double *RobotSettingsStruct::ThrusterCoefficientArray() {
    return this->_thrusterCoefficientArrayPtr;
}

double *RobotSettingsStruct::HandCoefficientArray() {
    return this->_handCoefficientArrayPtr;
}

uint16_t RobotSettingsStruct::ThrusterNumber() {
    return ((BaseRobotSettingsStruct *) this->_data)->ThrusterNumber;
}

uint16_t RobotSettingsStruct::HandFreedom() {
    return ((BaseRobotSettingsStruct *) this->_data)->HandFreedom;
}

uint16_t &RobotSettingsStruct::MaxMotorsSpeed() {
    return ((BaseRobotSettingsStruct *) this->_data)->MaxMotorSpeed;
}

uint16_t &RobotSettingsStruct::MotorsProtocol() {
    return ((BaseRobotSettingsStruct *) this->_data)->MotorsProtocol;
}

std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingStruct) {

    ostream << "[RobotSettingsStruct]" << std::endl;

    uint16_t thrusterNumber = ((BaseRobotSettingsStruct *) robotSettingStruct._data)->ThrusterNumber;
    ostream << "ThrustersNumber: " << thrusterNumber << std::endl;

    double *thrusterCoefficientArray = robotSettingStruct._thrusterCoefficientArrayPtr;
    ostream << "MoveCoefficientMatrix:" << std::endl;
    for (size_t i = 0; i < thrusterNumber; i++) {
        ostream << "[ ";
        for (size_t j = 0; j < 5; j++) {
            ostream << thrusterCoefficientArray[6 * i + j] << ", ";
        }
        ostream << thrusterCoefficientArray[6 * i + 5] << "]" << std::endl;
    }

    uint16_t handFreedom = ((BaseRobotSettingsStruct *) robotSettingStruct._data)->HandFreedom;
    ostream << "HandFreedom: " << handFreedom << std::endl;

    double *handArray = robotSettingStruct._handCoefficientArrayPtr;
    ostream << "HandCoefficientArray: [";
    for (size_t i = 0; i < handFreedom - 1; i++) {
        ostream << handArray[i] << ", ";
    }
    ostream << handArray[handFreedom - 1] << "]" << std::endl;

    uint16_t motorsProtocol = ((BaseRobotSettingsStruct *) robotSettingStruct._data)->MotorsProtocol;
    ostream << "MotorProtocol: ";
    switch (motorsProtocol) {
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
        default:
            std::cout << "Undefined type";
    }
    ostream << std::endl;

    uint16_t maxMotorSpeed = ((BaseRobotSettingsStruct *) robotSettingStruct._data)->MaxMotorSpeed;
    ostream << "MaxMotorSpeed: " << maxMotorSpeed << std::endl;

    return ostream;
}

