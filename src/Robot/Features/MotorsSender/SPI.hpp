#ifndef ROBOT_SPI_HPP
#define ROBOT_SPI_HPP
#include "./IMotorsSender.hpp"
#include "DataTransmissions/SPI/SPI.hpp"

#include <array>
#include <cstring>

namespace MotorsSender {

    class SPI final : public IMotorsSender {
    private:
        const DataTransmissions::SPI _spi;

    public:

        SPI(const char *path, uint32_t speed_hz) : _spi(path, speed_hz) {}

        bool SendMotorsStruct(const MotorsStruct &motorsStruct) const final {
            std::array<char, 2 * MotorsStructLenMessage> motorsMessage{};

            std::memcpy(motorsMessage.data() + 2, &motorsStruct, MotorsStructLen);
            std::memcpy(motorsMessage.data() + MotorsStructLenMessage + 2, &motorsStruct, MotorsStructLen);
            motorsMessage[0] = 's';
            motorsMessage[1] = 's';

            motorsMessage[MotorsStructLenMessage - 2] = 's';
            motorsMessage[MotorsStructLenMessage - 1] = 's';
            motorsMessage[MotorsStructLenMessage] = 's';
            motorsMessage[MotorsStructLenMessage + 1] = 's';

            motorsMessage[MotorsStructLenMessage * 2 - 2] = 's';
            motorsMessage[MotorsStructLenMessage * 2 - 1] = 's';

            _spi.ReadWrite((char *) motorsMessage.data(), nullptr, motorsMessage.size());

            return true;
        }
    };
}

#endif