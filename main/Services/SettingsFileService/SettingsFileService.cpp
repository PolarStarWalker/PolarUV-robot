#include "SettingsFileService.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <list>

SettingsStruct &SettingsStruct::operator=(SettingsStruct *right) {
    this->ThrustersNumber = right->ThrustersNumber;
    this->MaxMotorSpeed = right->MaxMotorSpeed;
    this->MotorsProtocol = right->MotorsProtocol;

    delete[] MoveCoefficientArray;
    this->MoveCoefficientArray = new int64_t[this->ThrustersNumber * 6];
    for (ssize_t i = 0; i < right->ThrustersNumber; i++) {
        this->MoveCoefficientArray[i] = right->MoveCoefficientArray[i];
    }

    return *this;
}

SettingsStruct::~SettingsStruct() {
    delete[] this->MoveCoefficientArray;
}

SettingsFileService::SettingsFileService(const char *fileName) {
    this->_fileName = fileName;
}

void SettingsFile::ReadFile(const char *fileName) {
    std::fstream file(fileName, std::fstream::in | std::fstream::out | std::fstream::app);

    file.seekg(0, std::fstream::end);
    this->TextLength = file.tellg();
    file.seekg(0, std::fstream::beg);

    if (this->TextLength < 1) {
        this->TextLength = DefaultSettingsTextLength;
        this->Text = new char[DefaultSettingsTextLength];
        std::memcpy(this->Text, DefaultSettingsText, DefaultSettingsTextLength);
        file << DefaultSettingsText << std::endl;
    } else {
        this->Text = new char[this->TextLength + 1];
        this->Text[this->TextLength] = '\n';
        file.read(this->Text, this->TextLength);
    }
    file.close();
}


inline void
FindPole(const int8_t *structFlag, int8_t *stateFlag, const SettingsStructEnumType type, ssize_t *i, const char *text,
         const char *string,
         size_t stringLength) {
    if (structFlag[type - 1] == 1) {
        ssize_t cmpValue = std::strncmp(text, string, stringLength);
        if (cmpValue == 0 && text[stringLength] == ' ') {
            *i += stringLength + 1;
            *stateFlag = type;
#ifndef NDEBUG
            std::cout << (int) *stateFlag;
#endif
        }
    }
}

void SettingsFileService::GetSettings(SettingsStruct *externalSettingsStruct) {

    SettingsFile settingsFile;
    settingsFile.ReadFile(_fileName);

    std::list<int64_t *> moveCoefficientArrayList;
    std::list<int64_t> handCoefficientsList;

#ifndef NDEBUG
    std::cout << "--------Read Text-------\n";
    std::cout << settingsFile.Text << std::endl;
    std::cout << "--------Parse text-------\n";
#endif

    int8_t structFlags[8] = {};
    memset(structFlags, 1, 5);
    int8_t stateFlags[8] = {};

    for (ssize_t i = 0; i < settingsFile.TextLength; i++) {

        ///comments cut-off
        if (settingsFile.Text[i] == '#') {
            i = std::strchr(&settingsFile.Text[i], '\n') - settingsFile.Text;
        }

        ///space or endl cut-off
        if (settingsFile.Text[i] == ' ') continue;

        ///find pole
        FindPole(structFlags, &stateFlags[0], IsTurnOn, &i, &settingsFile.Text[i], TurnOnString,
                 TurnOnStringLength);

        FindPole(structFlags, &stateFlags[0], MoveCoefficientArray, &i, &settingsFile.Text[i],
                 MoveCoefficientArrayString,
                 MoveCoefficientArrayStringLength);

        FindPole(structFlags, &stateFlags[0], HandCoefficientArray, &i, &settingsFile.Text[i],
                 HandCoefficientArrayString,
                 HandCoefficientArrayStringLength);

        FindPole(structFlags, &stateFlags[0], MaxMotorSpeed, &i, &settingsFile.Text[i], MaxMotorSpeedString,
                 MaxMotorSpeedStringLength);

        FindPole(structFlags, &stateFlags[0], MotorsProtocol, &i, &settingsFile.Text[i], MotorsProtocolString,
                 MotorsProtocolStringLength);



        ///find '=' if pole founded
        if (stateFlags[0] != 0 && settingsFile.Text[i] == '=' && settingsFile.Text[i + 1] == ' ') {
            stateFlags[1] = 1;
            continue;
        }

        ///parse arguments
        if (stateFlags[1]) {
            if (stateFlags[0] == IsTurnOn) {
                externalSettingsStruct->IsTurnOn = std::strncmp(&settingsFile.Text[i], "true", 4) == 0;
                externalSettingsStruct->IsTurnOn ?
                        structFlags[IsTurnOn - 1] = 0 : structFlags[IsTurnOn - 1] = 1;
                stateFlags[0] = 0;
                i = std::strchr(&settingsFile.Text[i], '\n') - settingsFile.Text;
#ifndef NDEBUG
                std::cout << '=' << externalSettingsStruct->IsTurnOn;
#endif
                continue;
            }
            if (stateFlags[0] == MoveCoefficientArray) {
                i = std::strchr(&settingsFile.Text[i], '{') - settingsFile.Text + 1;
                ssize_t endpoint = std::strchr(&settingsFile.Text[i], '}') - settingsFile.Text;
                for (; i < endpoint; i++) {


                    ssize_t ptr = std::strchr(&settingsFile.Text[i], '[') - settingsFile.Text + 1;

                    if (ptr < endpoint && ptr > 0) { i = ptr; }
                    else break;

                    int64_t *array = new int64_t[6];

                    size_t count = 0;

                    while (count < 5) {
                        array[count] = std::stol(&settingsFile.Text[i]);
                        count++;
                        i = std::strchr(&settingsFile.Text[i], ',') - settingsFile.Text + 1;
                    }
                    array[count] = std::stol(&settingsFile.Text[i]);

                    ptr = std::strchr(&settingsFile.Text[i], ']') - settingsFile.Text + 1;

                    if (ptr < settingsFile.TextLength && ptr > 0) { i = ptr; }
                    else break;

#ifndef NDEBUG
                    std::cout << "[";
                    for (size_t j = 0; j < 5; j++) {
                        std::cout << array[j] << ",";
                    }
                    std::cout << array[5] << "]\n";
#endif
                    moveCoefficientArrayList.push_back(array);
                }

                structFlags[MoveCoefficientArray - 1] = 0;
                stateFlags[0] = 0;
                continue;
            }

            if (stateFlags[0] == HandCoefficientArray) {
                i = std::strchr(&settingsFile.Text[i], '[') - settingsFile.Text + 1;
                const ssize_t length = std::strchr(&settingsFile.Text[i], ']') - settingsFile.Text + 1;
                std::cout << settingsFile.Text[i];
                for (; i < settingsFile.TextLength; i++) {
                    int64_t coefficient = std::stol(&settingsFile.Text[i]);
                    handCoefficientsList.push_back(coefficient);
                    size_t ptr = std::strchr(&settingsFile.Text[i], ',') - settingsFile.Text + 1;
                    if (ptr > 0 && ptr < settingsFile.TextLength) { i = ptr; }
                    else break;
                    if (i > length) break;
                }
                i = length;
                structFlags[HandCoefficientArray - 1] = 0;
                stateFlags[0] = 0;
                continue;
            }

            if (stateFlags[0] == MaxMotorSpeed) {
                externalSettingsStruct->MaxMotorSpeed = std::stol(&settingsFile.Text[i]);
                externalSettingsStruct->MaxMotorSpeed != -1 ?
                        structFlags[MaxMotorSpeed - 1] = 0 : structFlags[MaxMotorSpeed - 1] = 1;
                stateFlags[0] = 0;
                i = std::strchr(&settingsFile.Text[i], '\n') - settingsFile.Text;
#ifndef NDEBUG
                std::cout << '=' << externalSettingsStruct->MaxMotorSpeed;
#endif
                continue;
            }

            if (stateFlags[0] == MotorsProtocol) {
                externalSettingsStruct->MotorsProtocol = std::stol(&settingsFile.Text[i]);
                externalSettingsStruct->MotorsProtocol != -1 ?
                        structFlags[MotorsProtocol - 1] = 0 : structFlags[MotorsProtocol - 1] = 1;
                stateFlags[0] = 0;
                i = std::strchr(&settingsFile.Text[i], '\n') - settingsFile.Text;
#ifndef NDEBUG
                std::cout << '=' << externalSettingsStruct->MotorsProtocol;
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

#else
    }
#endif

    delete[] externalSettingsStruct->MoveCoefficientArray;
    externalSettingsStruct->MoveCoefficientArray = new int64_t[moveCoefficientArrayList.size() * 6];

    size_t counter = 0;
    for (int64_t *array : moveCoefficientArrayList) {
        for (size_t j = 0; j < 6; j++) {
            externalSettingsStruct->MoveCoefficientArray[j + counter * 6] = array[j];
        }
        delete[] array;
        counter++;
    }
    externalSettingsStruct->ThrustersNumber = counter;


    delete[] externalSettingsStruct->HandCoefficientArray;
    externalSettingsStruct->HandCoefficientArray = new int64_t[handCoefficientsList.size() * 6];

    counter = 0;
    for (int64_t coefficient: handCoefficientsList) {
        externalSettingsStruct->HandCoefficientArray[counter] = coefficient;
        counter++;
    }
    externalSettingsStruct->HandFreedom = counter;


    if (*((int64_t *) structFlags) != 0) {
        externalSettingsStruct->IsTurnOn = false;
    }

}

