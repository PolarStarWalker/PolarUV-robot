#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#define BNO055_ID 0xA0
#define BNO055_ADDRESS 0x29

#include <cstdint>
#include <array>
#include <shared_mutex>
#include "./BNO055.hpp"
#include "../Interfaces/II2CPeripheral.hpp"

#include "Filters/FiltersGroup.hpp"
#include "DataTransmissions/I2C/I2C.hpp"

class BNO055_I2C final : public II2CPeripheral {
public:

    static BNO055_I2C &GetInstance() {
        static BNO055_I2C bno(BNO055_ADDRESS);
        return bno;
    }

    BNO055_I2C(const BNO055_I2C &bno055) = delete;

    bool Init(const I2C *i2c) final;

    void SendOperationMode(BNO055::OperationMode mode) const;

    void UseExternalCrystal(bool useExtCrl);

    size_t DelayUs() const final;

    BNO055::Data GetData() const;

private:

    explicit BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode = BNO055::OPERATION_MODE_NDOF_FMC_OFF);

    BNO055::Data _data{};

    FiltersGroup<14> _filters;

    mutable std::shared_mutex _dataMutex;
    const I2C *i2c_{};
    uint16_t _sensorAddress;
    BNO055::OperationMode _operationMode;

    bool ReadData() final;

    bool Reload() final;

    inline void SetData(const BNO055::Data &data) {
        this->_dataMutex.lock();
        this->_data = data;
        this->_dataMutex.unlock();
    }
};

#endif
