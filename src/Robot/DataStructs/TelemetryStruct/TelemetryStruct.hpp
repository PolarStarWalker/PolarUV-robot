#ifndef ROBOT_TELEMETRYSTRUCT_HPP
#define ROBOT_TELEMETRYSTRUCT_HPP

struct TelemetryStruct {

    enum Position {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3
    };

    float Rotation[4]{};

    float Acceleration[3]{};

    float Depth = 0.0f;

    float Pressure = 0.0f;

    float BatteryVoltage = 0.0f;

    bool Euler = true;
};

inline std::ostream &operator<<(std::ostream &ostream, TelemetryStruct telemetryStruct) {

    ostream << "AngleOfRotation" << std::endl;

    if (!telemetryStruct.Euler)
        ostream << "W: " << telemetryStruct.Rotation[TelemetryStruct::W] << std::endl;

    ostream << "X: " << telemetryStruct.Rotation[TelemetryStruct::X] << std::endl
            << "Y: " << telemetryStruct.Rotation[TelemetryStruct::Y] << std::endl
            << "Z: " << telemetryStruct.Rotation[TelemetryStruct::Z] << std::endl;

    ostream << "Acceleration" << std::endl;
    ostream << "X: " << telemetryStruct.Acceleration[TelemetryStruct::X] << std::endl
            << "Y: " << telemetryStruct.Acceleration[TelemetryStruct::Y] << std::endl
            << "Z: " << telemetryStruct.Acceleration[TelemetryStruct::Z] << std::endl;

    ostream << "Depth: " << telemetryStruct.Depth << std::endl;
    ostream << "Pressure: " << telemetryStruct.Pressure << std::endl;
    ostream << "Voltage: " << telemetryStruct.BatteryVoltage << std::endl;

    return ostream;
}


extern TelemetryStruct TelemetryStructData;
constexpr size_t TelemetryStructLen = sizeof(TelemetryStructData);

#endif