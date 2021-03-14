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

class SettingsFile {
public:
	int64_t TextLength;
	char* Text = nullptr;

	bool SintaxisIsRight();
	~SettingsFile() { delete[] this->Text; }
};

class SettingsFileService
{
private:
	const char* _fileName;

	///Read data from file into text array
	SettingsFile ReadFile();
	///Create SettingsStruct from text array
	SettingsStruct CreateSettingsStruct(const SettingsFile* settingsFile);

	//char* CreateSettingsFile(SettingsFile* settingsFile);


public:

	SettingsFileService(char* fileName);

	///read file & update SettingsStruct;
	void GetSettings(SettingsStruct* externalSettingsStruct);

};

