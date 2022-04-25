#ifndef ROBOT_SPI_HPP
#define ROBOT_SPI_HPP
#include "./IMotorsSender.hpp"
#include "DataTransmissions/SPI/SPI.hpp"

#include <array>
#include <cstring>

namespace MotorsSender {

    class SPI final : public IMotorsSender {
    private:
        const DataTransmissions::SPI spi_;

    public:

        SPI(const char *path, uint32_t speed_hz) : spi_(path, speed_hz) {}

        inline bool SendMotorsStruct(const MotorsStruct &motorsStruct) const final {
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

            spi_.ReadWrite((char *) motorsMessage.data(), nullptr, motorsMessage.size());

            return true;
        }
    };
}

#endif