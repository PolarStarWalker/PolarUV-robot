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

	SettingsStruct& operator=(SettingsStruct* right);
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
	const char* _fileName;

	///Read data from file into text array
	FileText ReadFile();
	///Create SettingsStruct from text array
	SettingsStruct CreateSettingsStruct(FileText fileText);

public:

	SettingsFileService(char* fileName);

	///read file & update SettingsStruct;
	void GetSettings(SettingsStruct* externalSettingsStruct);

};

