#ifndef ROBOT_ISENSOR_HPP
#define ROBOT_ISENSOR_HPP

#include "../../DataTransmissions/I2C/I2C.hpp"

#include "./Event.hpp"
#include "./Coruitine.hpp"

class SensorHandler;

struct SensorContext;

class ISensor {
    friend SensorHandler;
    friend SensorContext;
protected:

    explicit ISensor(I2C &i2c) :
            i2c_(&i2c) {}

    I2C *i2c_;
};

struct SensorContext {

    enum StateEnum : uint8_t {
        Online,
        Offline
    };

    std::shared_ptr<ISensor> Sensor;
    SensorTask ReadData;
    SensorTask Init;
    TimerType Timer;
    StateEnum State;

    explicit SensorContext(std::shared_ptr<ISensor> i2cPeripheral, SensorTask &&init, SensorTask &&readData);
};

#endif