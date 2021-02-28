#pragma once
#include <fstream>


struct SettingsStruct {
	ssize_t thrustersNumber = -1;
	ssize_t maxMotorSpeed = -1;
	ssize_t motorsPrtocol = -1;
	int64_t* coefficientArray[6];
};


struct FileText {
	int64_t FileLength;
	char* FileText = nullptr;
};

class SettingsFileService
{
private:
	const std::string _fileName = "server.properties";


	///Read data from file into text array
	FileText ReadFile();
	///Create SettingsStruct from text array
	SettingsStruct* CreateSettingsStruct(FileText fileText);

public:


	~SettingsFileService();

	///read file & update SettingsStruct;
	SettingsStruct* UpdateData();

};

