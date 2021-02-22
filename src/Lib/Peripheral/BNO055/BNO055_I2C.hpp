#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#include <cstdint>
#include <array>
#include <shared_mutex>

#include "../PeripheralHandler/SensorHandler.hpp"

#include "./BNO055.hpp"
#include "./Filters/FiltersGroup.hpp"

class BNO055_I2C final : public ISensor {
public:

    explicit BNO055_I2C(I2C &i2c,
                        uint16_t sensorAddress = BNO055::ADDRESS,
                        BNO055::OperationMode mode = BNO055::OPERATION_MODE_NDOF);

    BNO055_I2C(const BNO055_I2C &bno055) = delete;

    BNO055::Data GetData() const;

private:
    BNO055::Data data_{};

    FiltersGroup<14> filters_;

    mutable std::mutex dataMutex_;

    uint16_t sensorAddress_;

    BNO055::OperationMode operationMode_;

    inline void SetData(const BNO055::Data &data) {
        std::lock_guard guard(dataMutex_);
        data_ = data;
    }

public:
    SensorTask Init();

    SensorTask ReadData();
};

#endif
