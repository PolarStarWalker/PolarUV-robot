#include "./StartSettings.hpp"
#include <boost/json.hpp>
#include <fstream>

#include <Math/SIPrefix.hpp>


boost::json::value parseFile(const std::string_view &path) {

    std::fstream file(path.cbegin(), std::ios_base::in);

    boost::json::parser parser;

    boost::json::error_code ec;

    if(!file.is_open()){
        throw "Settings can't open";
    }
    do {
        char buf[1024]{};
        file.read(buf, sizeof(buf));
        parser.write(buf, ec);
    } while (!file.eof());

    return parser.release();
}

StartSettings::StartSettings(const std::string_view &path) :
        mode_(ROV),
        motorsSenderId_(MotorsSender::Id::SPI),
        motorsSender_(nullptr) {

    auto json = parseFile(path);

    auto mode = json.at("mode").as_string();

    if (mode == "AUV") {
        mode_ = AUV;
    } else if (mode == "ROV") {
        mode_ = ROV;
    } else {
        //ToDo: exception
        throw "not found";
    }

    auto motorSender = json.at("motorSender").as_object();

    if(motorSender.at("mode") == "SPI"){
        motorsSenderId_ = MotorsSender::Id::SPI;
        motorsSenderPath_ = motorSender.at("path").as_string();
    }

    sensorsPath_ = json.at("sensors").as_object().at("path").as_string();

}

StartSettings::~StartSettings() {
    delete motorsSender_;
}

MotorsSender::IMotorsSender &StartSettings::GetMotorsSender() {
    switch (motorsSenderId_) {
        case MotorsSender::Id::SPI:
            motorsSender_ = new MotorsSender::SPI(motorsSenderPath_.c_str(), Mega(6));
            return *motorsSender_;
    }
    std::terminate();
}

StartSettings::Mode StartSettings::GetMode() {
    return mode_;
}

std::string_view StartSettings::GetSensorsPath() {
    return sensorsPath_;
}
