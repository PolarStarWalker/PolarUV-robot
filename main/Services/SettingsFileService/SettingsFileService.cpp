#include "SettingsFileService.hpp"
#include <string>

const char SettingsText[] = "#TurnOn Robot \
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

SettingsStruct& SettingsStruct::operator=(SettingsStruct* right)
{
	this->ThrustersNumber = right->ThrustersNumber;
	this->MaxMotorSpeed = right->MaxMotorSpeed;
	this->MotorsPrtocol = right->MotorsPrtocol;

	delete[] CoefficientArray;
	this->CoefficientArray = new int64_t[this->ThrustersNumber * 6];
	for (ssize_t i = 0; i < right->ThrustersNumber; i++) {
		this->CoefficientArray[i] = right->CoefficientArray[i];
	}

	return *this;
}

SettingsFileService::SettingsFileService(char* fileName)
{
	this->_fileName = fileName;
}

bool SettingsFile::SintaxisIsRight() {
	
	//flag[0] '{}' 
	//flag[1] '"'
	//Flag[2] '"'
	//Flag[3] :
	//Flag[4] ,

	int8_t flag[8] = {};

	for (ssize_t i = 0; i < this->TextLength; i++) {


	}

	if (this->TextLength > 0 && (int32_t)flag[0] == 0 && (int32_t)flag[4] == 0) return true;
	else return false;
}

SettingsFile SettingsFileService::ReadFile()
{
	SettingsFile settingsFile;

	std::fstream file(_fileName, std::ios_base::in);

	file.seekg(0, file.end);
	settingsFile.TextLength = file.tellg();
	file.seekg(0, file.beg);

	if (settingsFile.TextLength == -1) { settingsFile.TextLength == 0; }

	settingsFile.Text = new char[settingsFile.TextLength];
	file.read(settingsFile.Text, settingsFile.TextLength);

	file.close();

	std::cout<< settingsFile.SintaxisIsRight()<<std::endl;

	return settingsFile;
}


SettingsStruct SettingsFileService::CreateSettingsStruct(const SettingsFile* settingsFileStruct) {
	SettingsStruct settingsStruct;

	return settingsStruct;
}


void SettingsFileService::GetSettings(SettingsStruct* externalSettingsStruct) {
	SettingsFile settingsFile = ReadFile();
	*externalSettingsStruct = CreateSettingsStruct(&settingsFile);
}


