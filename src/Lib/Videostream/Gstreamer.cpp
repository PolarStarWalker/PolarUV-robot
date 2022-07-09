#include "Gstreamer.hpp"
#include <gst/gst.h>

using namespace lib::processing;

Gstreamer::Gstreamer() :
        src(gst_element_factory_make("v4l2src", "src")),
        h264parse(gst_element_factory_make("h264parse", "parser")),
        rtp(gst_element_factory_make("rtph264pay", "rtp")),
        gdp(gst_element_factory_make("gdppay", "gdp")),
        sink(gst_element_factory_make("udpsink", "sink")),
        group(gst_pipeline_new("rov-pipeline")) {

    gst_bin_add_many(GST_BIN (group), src, h264parse, rtp, gdp, sink, nullptr);
}

void Gstreamer::Start(const Settings &settings) {

    auto caps = gst_caps_new_simple("application/x-rtp",
                                    "media", G_TYPE_STRING, "video",
                                    "payload", G_TYPE_INT, 96,
                                    "clock-rate", G_TYPE_INT, 90000,
                                    "encoding-name", G_TYPE_STRING,"H264",
                                    nullptr);

    g_object_set(sink, "port", 8000, "host", settings.Ip.cbegin(), nullptr);

    g_object_set(src, "device", settings.DeviceName.cbegin(), "do-timestamp", FALSE, nullptr);

    gst_element_link_filtered(h264parse, rtp, caps);

    gst_element_link_many(src, h264parse, rtp, gdp, sink, nullptr);

    gst_element_set_state(group, GST_STATE_PLAYING);
}

void Gstreamer::Stop() {
    gst_element_set_state(group, GST_STATE_NULL);
}

Gstreamer::~Gstreamer() {
    Stop();

    gst_element_link_many(src, h264parse, rtp, gdp, sink, nullptr);
    g_object_unref(group);
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