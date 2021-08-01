#ifndef ROBOT_TELEMETRYSTRUCT_HPP
#define ROBOT_TELEMETRYSTRUCT_HPP

struct TelemetryStruct {
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

extern TelemetryStruct TelemetryStructData;
constexpr size_t TelemetryStructLen = sizeof(TelemetryStructData);

#endif