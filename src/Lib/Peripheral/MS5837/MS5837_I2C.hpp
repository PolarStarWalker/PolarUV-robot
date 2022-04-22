#ifndef ROBOT_MS5837_I2C_HPP
#define ROBOT_MS5837_I2C_HPP

#include "../PeripheralHandler/SensorHandler.hpp"
#include "./Filters/FiltersGroup.hpp"
#include "./MS5837.hpp"

#define MS5837_ADDRESS 0x76

class MS5837_I2C final : public ISensor {
public:

    explicit MS5837_I2C(uint16_t sensorAddress);

    void SetFluidDensity(double density);

    MS5837::Data GetData() const;

    //from sensor
    bool Init(const I2C *i2c, TimerType &timer) final;

private:

    double fluidDensity_ = 997.0; // Freshwater

    std::array<uint8_t, 8> sensorCalibration_;

    const I2C *i2c_{};

    mutable std::mutex dataMutex_;

    MS5837::Data data_;

    FiltersGroup<3> filters_;

    const uint16_t sensorAddress_;

    SensorTask ReadDataAsync;
    SensorTask InitSensorsAsync;

private:

    SensorTask ReadDataImpl();

    SensorTask InitSensorsImpl();

    static uint8_t CRC4(std::array<uint8_t, 8> &n_prom);

    static MS5837::Data Calculate(const MS5837::Measure &measure, const std::array<uint8_t, 8> &Calibration);

    bool ReadData(TimerType &timer) final;

    inline void SetData(const MS5837::Data &data) {
        std::lock_guard guard(dataMutex_);
        data_ = data;
    }
};

#endif