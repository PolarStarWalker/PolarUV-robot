#ifndef ROBOT_VIDEOSTREAM_HPP
#define ROBOT_VIDEOSTREAM_HPP

#include <gst/gstelement.h>

namespace lib::processing{

    class Gstreamer {
        GstElement* src;
        GstElement* h264parse;
        GstElement* rtp;
        GstElement* gdp;
        GstElement* sink;
        GstElement* group;

    public:
        Gstreamer();

    };

}

extern"C" void InitGstreamer();

#endif
