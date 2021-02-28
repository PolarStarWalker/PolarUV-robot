#pragma once
#include <fstream>
#include <mutex>


struct SettingsStruct {
	ssize_t thrustersNumber = -1;
	ssize_t maxMotorSpeed = -1;
	ssize_t motorsPrtocol = -1;
	int64_t* coefficientArray[6];
};


class SettingsFileService
{
private:
	std::fstream _file;
	std::string _fileName;
	std::mutex _settingStructMutex;

	int64_t _fileLength;
	char* _fileText = nullptr;
	SettingsStruct* _settingsStruct = nullptr;


	bool ReadFile();

public:

	SettingsFileService();
	

	~SettingsFileService();

};

