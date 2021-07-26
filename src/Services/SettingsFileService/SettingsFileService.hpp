#pragma once
#include <fstream>
#include <string>

constexpr char DefaultSettingsText[] = "#TurnOn Robot \
\nTurnOn = false\
\n\
\n#MoveCoefficientArray is a matrix where rows is thrusters that drone have\
\n#Columns is a move vectors F : x, F : y, F : z, M : z, M : y, M : z\
\nMoveCoefficientArray = {\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0],\
\n		[0, 0, 0, 0, 0, 0]\
\n}\
\n#Coefficients for each freedom of hand\
\nHandCoefficientArray = [ 0, 0 ] \
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
\nMotorsProtocol = 8\
\n";

constexpr ssize_t DefaultSettingsTextLength = sizeof(DefaultSettingsText);

constexpr char TurnOnString[] = "TurnOn";
constexpr size_t TurnOnStringLength = sizeof(TurnOnString)-1;

constexpr char MoveCoefficientArrayString[] = "MoveCoefficientArray";
constexpr size_t MoveCoefficientArrayStringLength = sizeof(MoveCoefficientArrayString) - 1;

constexpr char HandCoefficientArrayString[] = "HandCoefficientArray";
constexpr size_t HandCoefficientArrayStringLength = sizeof(HandCoefficientArrayString)-1;

constexpr char MaxMotorSpeedString[] = "MaxMotorSpeed";
constexpr size_t MaxMotorSpeedStringLength = sizeof(MaxMotorSpeedString)-1;

constexpr char MotorsProtocolString[] = "MotorsProtocol";
constexpr size_t MotorsProtocolStringLength = sizeof(MotorsProtocolString)-1;

enum SettingsStructEnumType{
    IsTurnOn = 1,
    MoveCoefficientArray = 2,
    MaxMotorSpeed = 3,
    MotorsProtocol = 4,
    HandCoefficientArray = 5,
};

struct OldSettingsStruct {

	ssize_t ThrustersNumber = -1;
	ssize_t MaxMotorSpeed = -1;
	ssize_t MotorsProtocol = -1;
	ssize_t HandFreedom = -1;
	double* MoveCoefficientArray = nullptr;
    double* HandCoefficientArray = nullptr;
    bool IsTurnOn = false;

	~OldSettingsStruct();

	OldSettingsStruct(OldSettingsStruct&& settingsStruct) noexcept ;

    OldSettingsStruct()= default;

	OldSettingsStruct& operator=(OldSettingsStruct* right);
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
    void ReadAndParseFile(OldSettingsStruct* externalSettingsStruct);
public:

	explicit SettingsFileService(const char* fileName);

    ///read file & update OldSettingsStruct;
	OldSettingsStruct GetSettings();
};


