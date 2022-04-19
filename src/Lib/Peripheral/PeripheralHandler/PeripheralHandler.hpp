#ifndef ROBOT_PERIPHERAL_HANDLER_HPP
#define ROBOT_PERIPHERAL_HANDLER_HPP

#include <thread>
#include <cstring>
#include <shared_mutex>
#include <list>
#include "../Interfaces/II2CPeripheral.hpp"

#include "../../DataTransmissions/UART/UART.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"

struct I2CSensorsContext {

    II2CPeripheral *I2CPeripheral;
    bool IsOnline;
    bool WaitTransmissions;
    int Timerfd;

    explicit I2CSensorsContext(II2CPeripheral *i2cPeripheral);
    ~I2CSensorsContext();
};

class PeripheralHandler {
private:

    static constexpr size_t MAX_SENSORS = 10;
    static constexpr int MAX_TIMEOUT_MS = 500;

    mutable std::mutex i2cMutex_;
    mutable std::mutex peripheralMutex_;

    mutable std::list<I2CSensorsContext> i2cPeripherals_;

    const I2C i2c_;

    int epollfd_;

    std::thread thread_;

    std::atomic<bool> notDone_;

public:

    explicit PeripheralHandler(std::string_view i2c_path);

    bool AddI2CSensor(II2CPeripheral &newSensor) const;

    void Start() const;

    void StartAsync() const;

    ~PeripheralHandler();
};

#endif