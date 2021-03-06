#pragma once
#include <fstream>
#include <iostream>

class SettingsStruct {
public:

	ssize_t ThrustersNumber = -1;
	ssize_t MaxMotorSpeed = -1;
	ssize_t MotorsPrtocol = -1;
	int64_t* CoefficientArray = nullptr;

	~SettingsStruct() {	delete[] CoefficientArray; }
};


class FileText {
public:
	int64_t FileLength;
	char* FileText = nullptr;

	~FileText() { delete[] this->FileText; }
};

class SettingsFileService
{
private:
	const std::string _fileName = "rov.properties";


	///Read data from file into text array
	FileText ReadFile();
	///Create SettingsStruct from text array
	SettingsStruct* CreateSettingsStruct(FileText fileText);

public:


	~SettingsFileService();

	///read file & update SettingsStruct;
	SettingsStruct* GetSettings();

};

