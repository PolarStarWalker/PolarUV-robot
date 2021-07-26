#pragma once
#include <cstdint>
#include <cstddef>

#include <iostream>

enum DShotMode : int8_t {
	DShot150 = 1,
	DShot300 = 2,
	DShot600 = 4,
	DShot1200 = 8
};

enum MoveVector : uint8_t {
    Fx = 0,
    Fy = 1,
    Fz = 2,
    Mx = 3,
    My = 4,
    Mz = 5
};

struct SensorsStruct {
	float AngleX = 0.0f;
	float AngleY = 0.0f;
	float AngleZ = 0.0f;
	float AngleW = 0.0f;

	float AccelerationX = 0.0f;
	float AccelerationY = 0.0f;
	float AccelerationZ = 0.0f;

	float Depth = 0.0f;

	float BatteryVoltage = 0.0f;

	bool Euler = true;
};

extern SensorsStruct SensorsStructData;
constexpr size_t SensorsStructLen = sizeof(SensorsStructData);

struct CommandsStruct {

	///Array prototype
	///{Fx, Fy, Fz, Mx, My, Mz}
	
	float VectorArray[6] = {};
	float TheHand[2] = {};
	float Camera[2] = {};
	bool MotorsLock = true;
	bool Stabilization = false;
};

extern CommandsStruct CommandsStructData;
constexpr size_t CommandsStructLen = sizeof(CommandsStructData);

struct MotorsStruct {
	/// Array prototype
	///{T1CH1, T1CH2, T1CH3, T1CH4, T2CH1, T2CH2, T3CH3, T4CH1, T4CH2, T5CH1, T5CH2, T5CH4}
	///or
	///{PA8,   PA9,   PA10,  PA11,  PA15,  PB3,   PB0,   PB6,   PB7,   PA0,   PA1,   PA3}
	
	uint16_t PacketArray[12] = {};

	///DShotMode applies to the entire timer,
	///it changes the operation of all channels of one timer
	///Array prototype
	///{T1, T2, T3, T4, T5}
	
	DShotMode TimerPrescaler = DShot300;
};

extern MotorsStruct MotorsStructData;
constexpr size_t MotorsStructArrayLength = sizeof(MotorsStructData.PacketArray);
constexpr size_t MotorsStructLen = sizeof(MotorsStructData);
constexpr size_t MotorsStructLenMessage = MotorsStructLen + 2;

struct SettingsStruct {
    double *MoveCoefficientArray = nullptr;
    double *HandCoefficientArray = nullptr;
    ssize_t MaxMotorSpeed = -1;
    int8_t ThrustersNumber = -1;
    int8_t MotorsProtocol = -1;
    int8_t HandFreedom = -1;
    bool IsTurnOn = false;
};

extern SettingsStruct SettingsStructData;
constexpr size_t SettingsStructLen = sizeof(SettingsStructData);

std::ostream& operator << (std::ostream &s, const CommandsStruct &c);
std::ostream& operator << (std::ostream &s, const MotorsStruct &c);
std::ostream& operator << (std::ostream &s, const SettingsStruct &c);