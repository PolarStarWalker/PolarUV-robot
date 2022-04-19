#include "Sensors.hpp"

using namespace app;

Sensors::Sensors(ssize_t id, std::string_view i2c) :
        lib::network::IService(id),
        peripheralHandler_(i2c),
        bno055_(BNO055_I2C::GetInstance()),
        ms5837_(MS5837_ADDRESS) {

    peripheralHandler_.AddI2CSensor(bno055_);
    peripheralHandler_.AddI2CSensor(ms5837_);
    peripheralHandler_.StartAsync();

}

lib::network::Response Sensors::Read(const std::string_view &data) {

    std::string outputData = Response::GetData(&Sensors::GetSensorsStruct, this);

    return {std::move(outputData), lib::network::Response::Ok, serviceId_};
}
