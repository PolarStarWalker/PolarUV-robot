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
	
	//flag '{}', flag'"'
	int8_t flagOne[4] = {};
	int8_t flagTwo[4] = {};

	for (ssize_t i = 0; i < this->TextLength; i++) {

		if (this->Text[i] == '{' && (int32_t)*flagOne == 0) flagOne[0] += 1;
		else if (this->Text[i] == '{' && (int32_t)*flagOne != 0) {
			flagOne[0] = -1;
			break;
		}

		if (this->Text[i] == '}' && flagOne[0] == 1) flagOne[0] -= 1;
		else if(this->Text[i] == '{' && (int32_t)*flagOne != 1) {
			flagOne[0] = -1;
			break;
		}


		if (this->Text[i] == '"' && flagOne[0] == 1 && flagOne[1] == 0) flagOne[1] += 1;
		else if (this->Text[i] == '"' && flagOne[0] == 1 && flagOne[1] == 0) {
			flagOne[1] -= 1;
			flagOne[2] = 1;
		}
		else if (this->Text[i] == '"' && flagOne[0] == 0) {
			flagOne[1] = -1;
			break;
		}



	}

	if (this->TextLength>0 && (int32_t)*flagOne == 0 && (int32_t)*flagTwo == 0) return true;
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


