#include "Lib.hpp"

#include <iostream>
#include <fstream>

void LoggerInitialize(){
#if DEBUG
    std::clog.rdbuf(std::cout.rdbuf());
#else
    //static std::fstream output("log.txt", std::ios_base::out | std::ios_base::trunc);
    //std::clog.rdbuf(output.rdbuf());
    std::clog.rdbuf(std::cout.rdbuf());
#endif
}

extern "C" void InitGstreamer() {

        gst_init(nullptr, nullptr);

        GST_PLUGIN_STATIC_DECLARE(coreelements);
        GST_PLUGIN_STATIC_REGISTER(coreelements);

        GST_PLUGIN_STATIC_DECLARE(udp);
        GST_PLUGIN_STATIC_REGISTER(udp);

        GST_PLUGIN_STATIC_DECLARE(rtp);
        GST_PLUGIN_STATIC_REGISTER(rtp);

        GST_PLUGIN_STATIC_DECLARE(autoconvert);
        GST_PLUGIN_STATIC_REGISTER(autoconvert);

        GST_PLUGIN_STATIC_DECLARE(gdp);
        GST_PLUGIN_STATIC_REGISTER(gdp);

        GST_PLUGIN_STATIC_DECLARE(playback);
        GST_PLUGIN_STATIC_REGISTER(playback);

        GST_PLUGIN_STATIC_DECLARE(app);
        GST_PLUGIN_STATIC_REGISTER(app);
}

void lib::Initialize() {
    LoggerInitialize();
    InitGstreamer();
}