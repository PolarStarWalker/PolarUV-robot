#ifndef ROBOT_ISENSOR_HPP
#define ROBOT_ISENSOR_HPP

#include "../../DataTransmissions/I2C/I2C.hpp"

#include "./Event.hpp"
#include "./Coruitine.hpp"

class SensorHandler;
class SensorContext;

class ISensor {
    friend SensorHandler;
    friend SensorContext;
protected:

    explicit ISensor(ssize_t period_us) :
            period_us_(period_us){}

    virtual bool ReadData(TimerType &timer) = 0;

    virtual bool Init(const I2C *i2c, TimerType &timer) = 0;

    const ssize_t period_us_;
};

struct SensorContext {

    enum StateEnum : uint8_t {
        Online,
        Offline
    };

    std::shared_ptr<ISensor> I2CPeripheral;
    TimerType Timer;
    StateEnum State;

    explicit SensorContext(std::shared_ptr<ISensor> i2cPeripheral);
};

#endif