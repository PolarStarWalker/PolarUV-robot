#include "Videostream.hpp"

#include <gst/gst.h>

using namespace lib::processing;

Gstreamer::Gstreamer() :
        src(gst_element_factory_make("v4l2src", "src")),
        h264parse(gst_element_factory_make("h264parse", "parser")),
        rtp(gst_element_factory_make("rtph264pay", "rtp")),
        gdp(gst_element_factory_make("gdppay", "gdp")),
        sink(gst_element_factory_make("udpsink", "sink")),
        group(gst_pipeline_new("rov-pipeline")){}

extern "C" {
void InitGstreamer() {

    gst_init(nullptr, nullptr);

//    GST_PLUGIN_STATIC_DECLARE(coreelements);
//    GST_PLUGIN_STATIC_REGISTER(coreelements);
//
//    GST_PLUGIN_STATIC_DECLARE(video4linux2);
//    GST_PLUGIN_STATIC_REGISTER(video4linux2);
//
//    GST_PLUGIN_STATIC_DECLARE(rtp);
//    GST_PLUGIN_STATIC_REGISTER(rtp);
//
//    GST_PLUGIN_STATIC_DECLARE(gdp);
//    GST_PLUGIN_STATIC_REGISTER(gdp);
//
//    GST_PLUGIN_STATIC_DECLARE(udp);
//    GST_PLUGIN_STATIC_REGISTER(udp);
}
}