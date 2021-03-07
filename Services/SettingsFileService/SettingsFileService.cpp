#include "SettingsFileService.hpp"

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

	std::fstream file(this->_fileName.c_str(), std::ios_base::in);

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

	std::cout << fileText.FileText;

	return settingsStruct;
}

void SettingsFileService::GetSettings(SettingsStruct* externalSettingsStruct) {
	*externalSettingsStruct = CreateSettingsStruct(ReadFile());
}


