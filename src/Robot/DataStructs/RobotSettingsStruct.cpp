#include "./RobotSettingsStruct/RobotSettingsStruct.hpp"

RobotSettingsStruct::RobotSettingsStruct(const std::vector<double> &copyMoveArray,
                                         const std::vector<double> &copyHandArray) {
    this->_length = ArraysOffset + copyMoveArray.size() * sizeof(double) + copyHandArray.size() * sizeof(double);
    this->_handArrayOffset = ArraysOffset + copyMoveArray.size() * sizeof(double);

    this->_data = new char[this->_length]{};

    std::memcpy(&_data[ArraysOffset], copyMoveArray.data(), copyMoveArray.size() * sizeof(double));
    std::memcpy(&_data[_handArrayOffset], copyHandArray.data(), copyHandArray.size() * sizeof(double));

    *((int16_t *) (this->_data + HandFreedomOffset)) = copyHandArray.size();
    *((int16_t *) (this->_data + ThrusterNumberOffset)) = copyMoveArray.size() / 6;
}

RobotSettingsStruct::RobotSettingsStruct(size_t thrustersNumber, size_t handFreedom) {
    this->_data = new char[ArraysOffset + thrustersNumber * 6 * sizeof(double) + handFreedom * sizeof(double)];
    this->_length = ArraysOffset + thrustersNumber * 6 * sizeof(double) + handFreedom * sizeof(double);
    this->_handArrayOffset = ArraysOffset + thrustersNumber * 6 * sizeof(double);
    *((int16_t *) (this->_data + ThrusterNumberOffset)) = thrustersNumber;
    *((int16_t *) (this->_data + HandFreedomOffset)) = handFreedom;
}

RobotSettingsStruct::RobotSettingsStruct() {
    this->_data = new char[ArraysOffset + 2 * sizeof(double)];
    *((int16_t *) (this->_data + ThrusterNumberOffset)) = 0;
    *((int16_t *) (this->_data + HandFreedomOffset)) = 0;
    this->_length = ArraysOffset + 2 * sizeof(double);
    this->_handArrayOffset = ArraysOffset + sizeof(double);
}

RobotSettingsStruct::RobotSettingsStruct(const RobotSettingsStruct &robotSettingsStruct) {
    this->_handArrayOffset = robotSettingsStruct._handArrayOffset;
    this->_length = robotSettingsStruct._length;
    this->_data = new char[this->_length];
    for (size_t i = 0; i < this->_length / 8; i++)
        ((uint64_t *) this->_data)[i] = ((uint64_t *) robotSettingsStruct._data)[i];
}

RobotSettingsStruct::RobotSettingsStruct(RobotSettingsStruct &&robotSettingsStruct) {
    this->_handArrayOffset = robotSettingsStruct._handArrayOffset;
    this->_length = robotSettingsStruct._length;
    this->_data = robotSettingsStruct._data;
    robotSettingsStruct._data = nullptr;
}

RobotSettingsStruct::~RobotSettingsStruct() {
    delete[] this->_data;
}

RobotSettingsStruct &RobotSettingsStruct::operator=(const RobotSettingsStruct &robotSettingsStruct)

noexcept {
    this->
            _length = robotSettingsStruct._length;
    this->
            _handArrayOffset = robotSettingsStruct._handArrayOffset;
    delete[] this->
            _data;
    this->
            _data = new char[this->_length];
    for (
            size_t i = 0;
            i < this->_length / 8; i++)
        ((uint64_t *) _data)[i] = ((uint64_t *) robotSettingsStruct._data)[i];
    return *this;
}

RobotSettingsStruct &RobotSettingsStruct::operator=(RobotSettingsStruct &&robotSettingsStruct)

noexcept {
    this->
            _handArrayOffset = robotSettingsStruct._handArrayOffset;
    this->
            _length = robotSettingsStruct._length;
    this->
            _data = robotSettingsStruct._data;
    robotSettingsStruct.
            _data = nullptr;
    return *this;
}


char *RobotSettingsStruct::Begin() {
    return this->_data;
}

char *RobotSettingsStruct::End() {
    return this->_data + this->_length;
}

size_t RobotSettingsStruct::Size() {
    return this->_length;
}

double *const RobotSettingsStruct::ThrusterCoefficientArray() {
    return (double *) (_data + ArraysOffset);
}

int16_t RobotSettingsStruct::ThrusterNumber() {
    return *((int16_t *) (_data + ThrusterNumberOffset));
}

double *const RobotSettingsStruct::HandCoefficientArray() {
    return (double *) (_data + _handArrayOffset);
}

int16_t RobotSettingsStruct::HandFreedom() {
    return *((int16_t *) (_data + HandFreedomOffset));
}

int16_t &RobotSettingsStruct::MaxMotorsSpeed() {
    return *((int16_t *) (_data + MaxMotorSpeedOffset));
}

int16_t &RobotSettingsStruct::MotorsProtocol() {
    return *((int16_t *) (_data + MotorsProtocolOffset));
}

std::ostream &operator<<(std::ostream &ostream, const RobotSettingsStruct &robotSettingStruct) {

    ostream << "[RobotSettingsStruct]" << std::endl;

    int16_t thrusterNumber = *((int16_t *) (robotSettingStruct._data + ThrusterNumberOffset));
    ostream << "ThrustersNumber: " << thrusterNumber << std::endl;

    double *moveArray = (double *) (robotSettingStruct._data + ArraysOffset);
    ostream << "MoveCoefficientMatrix:" << std::endl;
    for (size_t i = 0; i < thrusterNumber; i++) {
        ostream << "[ ";
        for (size_t j = 0; j < 5; j++) {
            ostream << moveArray[6 * i + j] << ", ";
        }
        ostream << moveArray[6 * i + 5] << "]" << std::endl;
    }

    int16_t handFreedom = *((int16_t *) (robotSettingStruct._data + HandFreedomOffset));
    ostream << "HandFreedom: " << handFreedom << std::endl;

    double *handArray = (double *) (robotSettingStruct._data + robotSettingStruct._handArrayOffset);
    ostream << "HandCoefficientArray: [";
    for (size_t i = 0; i < handFreedom - 1; i++) {
        ostream << handArray[i] << ", ";
    }
    ostream << handArray[handFreedom - 1] << "]" << std::endl;

    int16_t motorsProtocol = *((int16_t *) (robotSettingStruct._data + MotorsProtocolOffset));
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
    }
    ostream << std::endl;

    int16_t maxMotorSpeed = *((int16_t *) (robotSettingStruct._data + MaxMotorSpeedOffset));
    ostream << "MaxMotorSpeed: " << maxMotorSpeed << std::endl;

    return ostream;
}




