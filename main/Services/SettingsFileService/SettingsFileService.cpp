#include "SettingsFileService.hpp"
#include <string>
#include <cstring>

constexpr char SettingsText[] = "#TurnOn Robot \
\nTurnOn = false\
\n\
\n#The number of thrusters, that drone have\
\nThrustersNumber = 8\
\n\
\n#coefficientArray is a matrix where rows is thrustersNumber\
\n#Columns is a move vectors F : x, F : y, F : z, M : z, M : y, M : z\
\nCoeffitientArray = [\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0]\
\n]\
\n\
\n#The maximum speed of each motor(in rpm)\
\n#If vector is higher max motor speed, that all vectors casting to this speed\
\nMaxMotorSpeed = 4000\
\n\
\n#Data transfer protocol between ESC and microcontroller\
\n#0 - DShot150 - don't work\
\n#2 - DShot300 - should work\
\n#2 - DShot600 - should work\
\n#3 - DShot1200 - work\
\nMotorsPrtocol = 8";

constexpr ssize_t  SettingsTextLength = sizeof(SettingsText);

SettingsStruct& SettingsStruct::operator=(SettingsStruct* right)
{
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

SettingsFileService::SettingsFileService(const char* fileName)
{
	this->_fileName = fileName;
}

void SettingsFile::ReadFile(const char* fileName)
{
    std::fstream file(fileName, std::ios_base::in | std::ios_base::out);

    SettingsFile settingsFile;

	file.seekg(0, file.end);
	settingsFile.TextLength = file.tellg();
	file.seekg(0, file.beg);

	if(settingsFile.TextLength == -1){
        settingsFile.TextLength = SettingsTextLength;
        settingsFile.Text = new char[settingsFile.TextLength];
        std::memcpy(settingsFile.Text, SettingsText, SettingsTextLength);

        file.write(SettingsText,SettingsTextLength);
	}else{
        settingsFile.Text = new char[settingsFile.TextLength];
        file.read(settingsFile.Text, settingsFile.TextLength);
    }

	file.close();
}



void SettingsFileService::GetSettings(SettingsStruct* externalSettingsStruct) {
	SettingsFile settingsFile;
	settingsFile.ReadFile(_fileName);

	if (!settingsFile.SyntaxIsRight()){ externalSettingsStruct->IsTurnOn=false;	}
	else{

        int8_t flag[8] = {};

        for(ssize_t i = 0; i < settingsFile.TextLength; i++){

        }
	}
}


