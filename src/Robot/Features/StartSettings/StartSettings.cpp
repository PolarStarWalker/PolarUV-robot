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

ssize_t ParseFreq(boost::json::string& str){

    ssize_t factor;

    auto& lastSymbol = str[str.size() - 1];

    if( lastSymbol == 'M'){
        factor = Mega(1);
        lastSymbol = '\0';
    }else if( lastSymbol == 'K'){
        factor = Kilo(1);
        lastSymbol = '\0';
    }else if( lastSymbol <='9' && lastSymbol >= '0'){
        factor = 1;
    }else{
        //ToDo
        throw "exception";
    }

    return std::atof(str.c_str()) * factor;

}

StartSettings::StartSettings(const std::string_view &path) :
        mode_(ROV),
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
        auto pathStr = motorSender.at("path").as_string();
        auto freq = ParseFreq(motorSender.at("freq").as_string());
        motorsSender_ = new MotorsSender::SPI(pathStr.c_str(), freq);
    }

    sensorsPath_ = json.at("sensors").as_object().at("path").as_string();

}

StartSettings::~StartSettings() {
    delete motorsSender_;
}

MotorsSender::IMotorsSender &StartSettings::GetMotorsSender() {
    return *motorsSender_;
}

StartSettings::Mode StartSettings::GetMode() {
    return mode_;
}

std::string_view StartSettings::GetSensorsPath() {
    return sensorsPath_;
}
