#pragma once
#include <fstream>

constexpr char DefaultSettingsText[] = "#TurnOn Robot \
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
\nMotorsProtocol = 8";

constexpr ssize_t DefaultSettingsTextLength = sizeof(DefaultSettingsText);

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
    void ReadFile(const char* fileName);

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

