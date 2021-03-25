#pragma once
#include <fstream>

enum SettingsStructEnumType{
    IsTurnOn = 0,
    ThrusterNumber = 1,
    CoefficientArray = 2,
    MaxMotorSpeed = 3,
    MotorsProtocol = 4,
};


struct SettingsStruct {

	ssize_t ThrustersNumber = -1;
	ssize_t MaxMotorSpeed = -1;
	ssize_t MotorsProtocol = -1;
	int64_t* CoefficientArray = nullptr;
    bool IsTurnOn = false;

	~SettingsStruct();

	SettingsStruct& operator=(SettingsStruct* right);
};

struct SettingsFile {
	ssize_t TextLength = 0;
	char* Text = nullptr;

    ///Read data from file into text array
    static void ReadFile(const char* fileName);

	~SettingsFile() { delete[] this->Text; }
};

class SettingsFileService
{
private:
	const char* _fileName;

public:

	explicit SettingsFileService(const char* fileName);

	///read file & update SettingsStruct;
	void GetSettings(SettingsStruct* externalSettingsStruct);

};

