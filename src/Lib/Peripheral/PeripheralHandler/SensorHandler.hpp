#ifndef ROBOT_PERIPHERAL_HANDLER_HPP
#define ROBOT_PERIPHERAL_HANDLER_HPP

#include <thread>
#include <cstring>
#include <shared_mutex>
#include <vector>

#include "./ISensor.hpp"

#include "../../DataTransmissions/I2C/I2C.hpp"

class SensorHandler {
private:

    static constexpr size_t MAX_SENSORS = 10;
    static constexpr int MAX_TIMEOUT_MS = 500;

    std::vector<SensorContext> sensors_;

    I2C i2c_;

    EventTracker eventTracker_;

    mutable std::thread thread_;

    std::atomic<bool> notDone_;

    bool RegisterSensor(const std::shared_ptr<ISensor> &newSensor, SensorTask &&init, SensorTask &&readData);

public:

    explicit SensorHandler(std::string_view i2c_path);

    ~SensorHandler();

    void Start();

    void StartAsync();

    template<class T, typename ... Args>
    requires(std::convertible_to<T *, ISensor *>)
    std::shared_ptr<T> CreateSensor(Args && ... args){

        auto sensor = std::make_shared<T>(i2c_, std::forward<Args>(args)...);

        RegisterSensor(sensor, sensor->Init(), sensor->ReadData());

        return sensor;
    }
};

#endif