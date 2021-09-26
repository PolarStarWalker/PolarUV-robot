#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#define BNO055_ID 0xA0
#define BNO055_ADDRESS 0x29

#include <cstdint>
#include <array>
#include <shared_mutex>
#include "./BNO055.hpp"
#include "../Interfaces/II2CPeripheral.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"

class BNO055_I2C : public II2CPeripheral {
public:
    explicit BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode = BNO055::OPERATION_MODE_NDOF_FMC_OFF);

    bool Init(const I2C* i2c) final;

    void SendOperationMode(BNO055::OperationMode mode);

    void UseExternalCrystal(bool useExtCrl);

    BNO055::Data GetData() const;

private:
    const I2C *_i2c;
    uint16_t _sensorAddress;
    BNO055::OperationMode _operationMode{};
    BNO055::Data _data{};
    mutable std::shared_mutex _dataMutex;

    bool ReadData() final;

    bool Reload() final;

    inline void SetData(const BNO055::Data& data){
        this->_dataMutex.lock();
        this->_data = data;
        this->_dataMutex.unlock();
    }
};

#endif
