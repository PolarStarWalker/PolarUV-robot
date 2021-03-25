#include "SettingsFileService.hpp"
#include <string>
#include <cstring>
#include <iostream>

SettingsStruct &SettingsStruct::operator=(SettingsStruct *right) {
    this->ThrustersNumber = right->ThrustersNumber;
    this->MaxMotorSpeed = right->MaxMotorSpeed;
    this->MotorsProtocol = right->MotorsProtocol;

    delete[] CoefficientArray;
    this->CoefficientArray = new int64_t[this->ThrustersNumber * 6];
    for (ssize_t i = 0; i < right->ThrustersNumber; i++) {
        this->CoefficientArray[i] = right->CoefficientArray[i];
    }

    return *this;
}

SettingsStruct::~SettingsStruct() {
    delete[] this->CoefficientArray;
}

SettingsFileService::SettingsFileService(const char *fileName) {
    this->_fileName = fileName;
}

void SettingsFile::ReadFile(const char *fileName) {
    std::fstream file(fileName, std::ios_base::in | std::ios_base::out);

    file.seekg(0, std::fstream::end);
    this->TextLength = file.tellg();
    file.seekg(0, std::fstream::beg);

    if (this->TextLength == -1) {

        this->TextLength = DefaultSettingsTextLength;
        this->Text = new char[this->TextLength];
        std::memcpy(this->Text, DefaultSettingsText, DefaultSettingsTextLength);

        file.write(DefaultSettingsText, DefaultSettingsTextLength);
    } else {
        this->Text = new char[this->TextLength];
        file.read(this->Text, this->TextLength);
    }

    file.close();
}


void SettingsFileService::GetSettings(SettingsStruct *externalSettingsStruct) {
    SettingsFile settingsFile;
    settingsFile.ReadFile(_fileName);

    int8_t flag[8];
    memset(flag, 1, 8);
    int8_t commentFlag = 0;

    for (ssize_t i = 0; i < settingsFile.TextLength; i++) {

        ///comments cut-off
        if (settingsFile.Text[i] == '#') { commentFlag = 1; }

        if (commentFlag == 1 && settingsFile.Text[i] != '\n') {
            continue;
        } else if (commentFlag == 1 && settingsFile.Text[i] == '\n') {
            commentFlag = 0;
            continue;
        }

        std::cout<<settingsFile.Text[i];

    }

    if ((int64_t) flag[0] != 0) {
        externalSettingsStruct->IsTurnOn = false;
    }

}


