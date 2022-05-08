#ifndef ROBOT_GSTREAMER_HPP
#define ROBOT_GSTREAMER_HPP

#include <gst/gstelement.h>
#include <ostream>

namespace lib::processing {

    class Gstreamer {
        GstElement *src;
        GstElement *h264parse;
        GstElement *rtp;
        GstElement *gdp;
        GstElement *sink;
        GstElement *group;

    public:
        Gstreamer();

        void Start();
        void Stop();

        friend std::ostream &operator<<(std::ostream &out, const Gstreamer &gst) {

            out << "[GST STRUCTURE]"
                << "\nsrc:       " << gst.src
                << "\nh264parse: " << gst.h264parse
                << "\nrtp:       " << gst.rtp
                << "\ngdp:       " << gst.gdp
                << "\nsink:      " << gst.sink
                << "\npipeline:  " << gst.group;

            return out;
        }

    };

}

extern "C" void InitGstreamer();

#endif
