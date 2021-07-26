#include "DataStruct.hpp"

std::ostream &operator<<(std::ostream &s, const CommandsStruct &c) {
    s << "VectorArray: ["
      << c.VectorArray[Fx] << ", "
      << c.VectorArray[Fy] << ", "
      << c.VectorArray[Fz] << ", "
      << c.VectorArray[Mx] << ", "
      << c.VectorArray[My] << ", "
      << c.VectorArray[Mz] << "]"
      << std::endl

      << "The Hand: ["
      << c.TheHand[0] << ", "
      << c.TheHand[1] << "]"
      << std::endl

      << "Camera: ["
      << c.Camera[0] << ", "
      << c.Camera[1] << "]"
      << std::endl

      << "MotorsLock: "
      << c.MotorsLock << std::endl

      << "Stabilization: "
      << c.Stabilization << std::endl;

    return s;
}

std::ostream &operator<<(std::ostream &s, const MotorsStruct &c) {
    s << "MotorsCommand: ["
      << c.PacketArray[0] << ", "
      << c.PacketArray[1] << ", "
      << c.PacketArray[2] << ", "
      << c.PacketArray[3] << ", "
      << c.PacketArray[4] << ", "
      << c.PacketArray[5] << ", "
      << c.PacketArray[6] << ", "
      << c.PacketArray[7] << ", "
      << c.PacketArray[8] << ", "
      << c.PacketArray[9] << ", "
      << c.PacketArray[10] << ", "
      << c.PacketArray[11] << "]"
      << std::endl

      << "Time Prescaler: ["
      << c.TimerPrescaler << "]"
      << std::endl;

    return s;
}

std::ostream &operator<<(std::ostream &ostream, const SettingsStruct &settingsStruct) {

    ostream << "ThrusterNumber: " << settingsStruct.ThrustersNumber << std::endl;

    ostream << "MoveCoefficientMatrix:" << std::endl;
    for (size_t i = 0; i < settingsStruct.ThrustersNumber; i++) {
        ostream << "[ ";
        for (size_t j = 0; j < 5; j++) {
            ostream << settingsStruct.MoveCoefficientArray[6 * i + j] << ", ";
        }
        ostream << settingsStruct.MoveCoefficientArray[6 * i + 6] << "]" << std::endl;
    }

    ostream << "HandFreedom: " << (ssize_t) settingsStruct.HandFreedom << std::endl;
    ostream << "HandCoefficientArray: [";
    for (size_t i = 0; i < settingsStruct.HandFreedom - 1; i++) {
        ostream << settingsStruct.HandCoefficientArray[i] << ", ";
    }
    ostream << settingsStruct.HandCoefficientArray[settingsStruct.HandFreedom - 1] << ", ]" << std::endl;

    std::cout << "MotorProtocol: " << (long) settingsStruct.MotorsProtocol << std::endl;
    std::cout << "MaxMotorSpeed: " << settingsStruct.MaxMotorSpeed << std::endl;

    return ostream;
}