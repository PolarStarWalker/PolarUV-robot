#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#define BNO055_ID 0xA0
#define BNO055_ADDRESS 0x29

#include <cstdint>
#include <array>
#include <shared_mutex>

#include "../PeripheralHandler/SensorHandler.hpp"

#include "./BNO055.hpp"
#include "./Filters/FiltersGroup.hpp"

class BNO055_I2C final : public ISensor {
public:

    explicit BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode = BNO055::OPERATION_MODE_NDOF_FMC_OFF);

    BNO055_I2C(const BNO055_I2C &bno055) = delete;

    bool Init(const I2C *i2c, TimerType &timer) final;

    BNO055::Data GetData() const;

    SensorTask ReadDataImpl();

    SensorTask InitSensorsImpl();

private:

    SensorTask ReadDataAsync;
    SensorTask InitSensorsAsync;

    const I2C *i2c_{};

    BNO055::Data data_{};

    FiltersGroup<14> filters_;

    mutable std::mutex dataMutex_;

    uint16_t sensorAddress_;

    BNO055::OperationMode operationMode_;

    bool ReadData(TimerType &timer) final;

    inline void SetData(const BNO055::Data &data) {
        std::lock_guard guard(dataMutex_);
        data_ = data;
    }
};

#endif
