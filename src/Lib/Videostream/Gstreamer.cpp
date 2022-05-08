#include "Gstreamer.hpp"

#include <gst/gst.h>

using namespace lib::processing;

Gstreamer::Gstreamer() :
        src(gst_element_factory_make("v4l2src", "src")),
        h264parse(gst_element_factory_make("h264parse", "parser")),
        rtp(gst_element_factory_make("rtph264pay", "rtp")),
        gdp(gst_element_factory_make("gdppay", "gdp")),
        sink(gst_element_factory_make("udpsink", "sink")),
        group(gst_pipeline_new("rov-pipeline")){

    g_object_set(src, "device", "/dev/video2", nullptr);

    g_object_set(sink, "port", 8000, "host", "192.168.1.211", nullptr);

    gst_bin_add_many(GST_BIN (group), src, h264parse, rtp, gdp, sink, nullptr);

    gst_element_link_many(src, h264parse, rtp, gdp, sink, nullptr);

    gst_element_set_state(group, GST_STATE_READY);
}

void Gstreamer::Start() {
    gst_element_set_state(group, GST_STATE_PLAYING);
}

void Gstreamer::Stop() {
    gst_element_set_state(group, GST_STATE_PAUSED);
}

extern "C" {
void InitGstreamer() {

    gst_init(nullptr, nullptr);

    GST_PLUGIN_STATIC_DECLARE(coreelements);
    GST_PLUGIN_STATIC_REGISTER(coreelements);

    GST_PLUGIN_STATIC_DECLARE(video4linux2);
    GST_PLUGIN_STATIC_REGISTER(video4linux2);

    GST_PLUGIN_STATIC_DECLARE(videoparsersbad);
    GST_PLUGIN_STATIC_REGISTER(videoparsersbad);

    GST_PLUGIN_STATIC_DECLARE(rtp);
    GST_PLUGIN_STATIC_REGISTER(rtp);

    GST_PLUGIN_STATIC_DECLARE(gdp);
    GST_PLUGIN_STATIC_REGISTER(gdp);

    GST_PLUGIN_STATIC_DECLARE(udp);
    GST_PLUGIN_STATIC_REGISTER(udp);
}
}