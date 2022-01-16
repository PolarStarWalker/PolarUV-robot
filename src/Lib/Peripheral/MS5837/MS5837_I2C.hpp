#ifndef ROBOT_MS5837_I2C_HPP
#define ROBOT_MS5837_I2C_HPP

#include "../Interfaces/II2CPeripheral.hpp"
#include "./MS5837.hpp"
#include "./Filters/FiltersGroup.hpp"

#define MS5837_ADDRESS 0x76

class MS5837_I2C final : public II2CPeripheral {
public:

    explicit MS5837_I2C(uint16_t sensorAddress);

    ~MS5837_I2C();

    void SetFluidDensity(double density);

    MS5837::Data GetData() const;

    size_t DelayUs() const final;

    //from sensor
    bool Init(const I2C *i2c) final;

private:

    mutable std::shared_mutex dataMutex_;

    MS5837::Data data_;

    FiltersGroup<3> filters_;

    const I2C *i2c_;

    uint16_t C_[8]{};
    uint32_t d1Pressure_{};
    uint32_t d2Temperature_{};
    int32_t _temperature{};
    int32_t _p{};
    uint16_t sensorAddress_;

    double fluidDensity_ = 997.0; // Freshwater

    static uint8_t CRC4(uint16_t *n_prom);

    void Calculate();

    bool ReadData() final;

    bool Reload() final;

    inline void SetData(const MS5837::Data &data) {
        std::lock_guard guard(dataMutex_);
        data_ = data;
    }
};

#endif