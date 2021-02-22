#include "Sensors.hpp"

using namespace app;

using ResponseBufferType = lib::network::IService::ResponseBufferType;

Sensors::Sensors(ssize_t id, const std::string_view i2c) :
        lib::network::IService(id),
        sensorHandler_(i2c),
        bno055_(sensorHandler_.CreateSensor<BNO055_I2C>(BNO055::ADDRESS)),
        ms5837_(sensorHandler_.CreateSensor<MS5837::MS5837_I2C>(MS5837::ADDRESS)){
    sensorHandler_.StartAsync();
}

ResponseBufferType Sensors::Read() {

    std::string outputData(sizeof(SensorsStruct), 0);

    SensorsStruct& bufferBegin = *((SensorsStruct*) &outputData[0]);

    bufferBegin = GetSensorsStruct();

    return outputData;
}
