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

#ifndef NDEBUG
    std::cout << "--------Read Text-------\n";
    std::cout<<settingsFile.Text<<std::endl;
    std::cout << "Read Text-------\n";
#endif

    int8_t structFlags[8];
    memset(structFlags, 1, 5);
    int8_t stateFlags[8] = {};

    size_t continueCount = 0;

    for (ssize_t i = 0; i < settingsFile.TextLength; i++) {

        ///comments cut-off
        if (settingsFile.Text[i] == '#') { stateFlags[0] = 1; }

        if (stateFlags[0] == 1 && settingsFile.Text[i] != '\n') {
            continue;
        } else if (stateFlags[0] == 1 && settingsFile.Text[i] == '\n') {
            stateFlags[0] = 0;
            continue;
        }

        ///cut-off found pole
        if (continueCount != 0) {
            continueCount--;
            continue;
        }

        ///space cut-off
        if (settingsFile.Text[i] == ' ') continue;

        ///find pole
        if (structFlags[IsTurnOn - 1] == 1) {
            ssize_t cmpValue = std::strncmp(&settingsFile.Text[i], TurnOnString, TurnOnStringLength);
            if (cmpValue == 0 && settingsFile.Text[i + TurnOnStringLength] == ' ') {
                continueCount = TurnOnStringLength;
                stateFlags[1] = IsTurnOn;
#ifndef NDEBUG
                std::cout << (int) stateFlags[1];
#endif
                continue;
            }
        }

        if (structFlags[CoefficientArray - 1] == 1) {
            ssize_t cmpValue = std::strncmp(&settingsFile.Text[i], CoefficientArrayString,
                                            CoefficientArrayStringLength);
            if (cmpValue == 0 && settingsFile.Text[i + CoefficientArrayStringLength] == ' ') {
                continueCount = CoefficientArrayStringLength;
                stateFlags[1] = CoefficientArray;
#ifndef NDEBUG
                std::cout << (int) stateFlags[1];
#endif
                continue;
            }
        }

        if (structFlags[MaxMotorSpeed - 1] == 1) {
            ssize_t cmpValue = std::strncmp(&settingsFile.Text[i], MaxMotorSpeedString, MaxMotorSpeedStringLength);
            if (cmpValue == 0 && settingsFile.Text[i + MaxMotorSpeedStringLength] == ' ') {
                continueCount = MaxMotorSpeedStringLength;
                stateFlags[1] = MaxMotorSpeed;
#ifndef NDEBUG
                std::cout << (int) stateFlags[1];
#endif
                continue;
            }
        }

        if (structFlags[MotorsProtocol - 1] == 1) {
            ssize_t cmpValue = std::strncmp(&settingsFile.Text[i], MotorsProtocolString, MotorsProtocolStringLength);
            if (cmpValue == 0 && settingsFile.Text[i + MotorsProtocolStringLength] == ' ') {
                continueCount = MotorsProtocolStringLength;
                stateFlags[1] = MotorsProtocol;
#ifndef NDEBUG
                std::cout << (int) stateFlags[1];
#endif
                continue;
            }
        }

        if (stateFlags[1] != 0 && settingsFile.Text[i] == '=' && settingsFile.Text[i + 1] == ' ') {
            stateFlags[2] = 1;
            continue;
        }

        if (stateFlags[2]) {
            if (stateFlags[1] == IsTurnOn) {
                externalSettingsStruct->IsTurnOn = std::strncmp(&settingsFile.Text[i], "true", 4) == 0;
                externalSettingsStruct->IsTurnOn ? structFlags[0] = 0 : structFlags[0] = 1;
                stateFlags[1] = 0;
                continueCount = 3;
#ifndef NDEBUG
                std::cout << '=' << externalSettingsStruct->IsTurnOn;
#endif
                continue;
            }
        }
#ifndef NDEBUG
        std::cout << settingsFile.Text[i];
    }
    std::cout << std::endl;

    std::cout << '\n' << "-----StructFlagsBegin------" << std::endl;

    for (size_t i = sizeof(structFlags); i > 0; i--) {
        std::cout << (int) structFlags[i - 1];
    }

    std::cout << '\n' << "-----StructFlagsEnd------" << std::endl;
#else
    }
#endif

    if (*((int64_t *) structFlags) != 0) {
        externalSettingsStruct->IsTurnOn = false;
    }

}


