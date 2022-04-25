#include "Sensors.hpp"

using namespace app;

Sensors::Sensors(ssize_t id, const std::string_view i2c) :
        lib::network::IService(id),
        sensorHandler_(i2c),
        bno055_(sensorHandler_.CreateSensor<BNO055_I2C>(BNO055::ADDRESS)),
        ms5837_(sensorHandler_.CreateSensor<MS5837::MS5837_I2C>(MS5837::ADDRESS))
//        ms5837_(nullptr)
{
    sensorHandler_.StartAsync();
}

lib::network::Response Sensors::Read(const std::string_view &data) {

    std::string outputData = Response::GetData(&Sensors::GetSensorsStruct, this);

    return {std::move(outputData), lib::network::Response::Ok, serviceId_};
}
