#include "Gstreamer.hpp"
#include <gst/gst.h>
#include <gst/gstdeviceprovider.h>
#include <filesystem>
#include <iostream>
#include <boost/regex.hpp>

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
                                    "encoding-name", G_TYPE_STRING, "H264",
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

const boost::regex filter("(/dev/video)([0-9]{2})");
boost::match_results<std::string::const_iterator> results;

void foo(gpointer data, [[maybe_unused]] gpointer user_data) {

    auto device = reinterpret_cast<GstDevice *>(data);

    auto caps = gst_device_get_caps(device);

    auto structure = gst_caps_get_structure(caps, 0);

    auto name = gst_structure_get_name(structure);

    bool flag = g_str_has_prefix(name, "video/x-h264") == 1;

    std::cout << flag << std::endl;

    auto prop = gst_device_get_properties(device);

    auto path = gst_structure_get_string(prop, "device.path");

    std::cout << path << std::endl;

}

std::string Gstreamer::ScanCameras() {

    std::string result{};

    auto deviceProvider = gst_device_provider_factory_get_by_name("v4l2deviceprovider");

    auto devices = gst_device_provider_get_devices(deviceProvider);

    g_list_foreach(devices, (GFunc) foo, nullptr);

    for (const auto &entry: std::filesystem::directory_iterator("/dev/")) {

        auto str = entry.path().string();

        if (!boost::regex_match(str, results, filter))
            continue;

        result += str + ' ';

    }

    return result;
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