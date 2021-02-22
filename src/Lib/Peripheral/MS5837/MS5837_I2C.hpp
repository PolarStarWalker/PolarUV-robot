#ifndef ROBOT_MS5837_I2C_HPP
#define ROBOT_MS5837_I2C_HPP

#include "../PeripheralHandler/SensorHandler.hpp"
#include "./Filters/FiltersGroup.hpp"
#include "./MS5837.hpp"


namespace MS5837{
class MS5837_I2C final : public ISensor {
public:

    explicit MS5837_I2C(I2C &i2c, uint16_t sensorAddress = ADDRESS);

    void SetFluidDensity(double density);

    Data GetData() const;

private:

    double fluidDensity_ = 997.0; //

    std::array<uint16_t, 8> sensorCalibration_;

    mutable std::mutex dataMutex_;

    Data data_;

    FiltersGroup<3> filters_;

    const uint16_t sensorAddress_;

private:

    static uint8_t CRC4(std::array<uint16_t, 8> &n_prom);

    static Data Calculate(const Measure &measure, const std::array<uint16_t, 8> &Calibration);

    inline void SetData(const Data &data) {
        std::lock_guard guard(dataMutex_);
        data_ = data;
    }

    //from ISensor
public:

    SensorTask ReadData();

    SensorTask Init();
};

}
#endif