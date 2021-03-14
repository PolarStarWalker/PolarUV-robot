#include "SettingsFileService.hpp"
#include <string>

const char SettingsText[] = "{ \
\n\"turnOn\": false,\ 
\n\ 
\n\"//01\": \"The number of thrusters, that drone have\",\
\n\"thrustersNumber\" : 8,\
\n\
\n\"//02\" : \"coefficientArray is a matrix where rows is thrustersNumber\",\
\n\"//03\" : \"Columns is a move vectors F:x,F:y,F:z,M:z,M:y,M:z\",\
\n\"coeffitientArray\" : [\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0],\
\n	[0, 0, 0, 0, 0, 0]\
\n],\
\n\
\n\"//04\": \"The maximum speed of each motor (in rpm)\",\
\n\"//05\" : \"If vector is higher max motor speed, that all vectors casting to this speed\",\
\n\"maxMotorSpeed\" : 4000,\
\n\
\n\"//06\" : \"Data transfer protocol between ESC and microcontroller\",\
\n\"//08\" : \"1 - DShot150  - don't work\",\
\n\"//09\" : \"2 - DShot300  - should work\",\
\n\"//10\" : \"4 - DShot600  - should work\",\
\n\"//11\" : \"8 - DShot1200 - work\",\
\n\"motorsPrtocol\" : 8\
\n}\
\n";

SettingsFileService::SettingsFileService(char* fileName)
{
	this->_fileName = fileName;
}

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


FileText SettingsFileService::ReadFile()
{
	FileText fileText;

	std::fstream file(_fileName, std::ios_base::in);

	file.seekg(0, file.end);
	fileText.FileLength = file.tellg();
	file.seekg(0, file.beg);

	fileText.FileText = new char[fileText.FileLength];
	file.read(fileText.FileText, fileText.FileLength);

	file.close();

	return fileText;
}


SettingsStruct SettingsFileService::CreateSettingsStruct(FileText fileText) {
	SettingsStruct settingsStruct;

	std::cout << SettingsText;

	return settingsStruct;
}


void SettingsFileService::GetSettings(SettingsStruct* externalSettingsStruct) {
	*externalSettingsStruct = CreateSettingsStruct(ReadFile());
}


