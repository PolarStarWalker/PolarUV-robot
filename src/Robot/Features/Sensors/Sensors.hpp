#ifndef ROBOT_SENSORS_HPP
#define ROBOT_SENSORS_HPP

#include "./DataTransmissions/TcpSession/TcpSession.hpp"
#include "./Peripheral/PeripheralHandler/PeripheralHandler.hpp"

class Sensors final  : public lib::network::IService{
public:
    Sensors(ssize_t id, std::string_view i2c) : lib::network::IService(id){};

    Sensors(const Sensors&) = delete;
    Sensors(Sensors&&) = delete;
    Sensors& operator=(const Sensors&) = delete;
    Sensors& operator=(Sensors&&) = delete;

private:

    PeripheralHandler peripheralHandler_;

};


#endif
