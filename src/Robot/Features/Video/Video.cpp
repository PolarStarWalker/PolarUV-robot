#include <unistd.h>
#include <fstream>
#include "Video.hpp"
#include "./VideoMessage.pb.h"

using namespace app;


namespace auv {
    constexpr std::string_view auv_pipeline = R"(v4l2src device=/dev/video2 do-timestamp=true
        ! video/x-h264,width=1920,height=1080,framerate=30/1
        ! h264parse
        ! v4l2h264dec
        ! autovideoconvert
        ! appsink sync=false)";

    constexpr std::string_view auv_stream = R"(appsrc
        ! videoconvert
        ! v4l2h264enc output-io-mode=4  extra-controls="encode,frame_level_rate_control_enable=1,h264_profile=0,h264_level=14,video_bitrate=2500000;"
        ! h264parse
        ! matroskamux
        ! filesink location=/home/pi/files/test.mkv)";
}

Video::Video(ssize_t id) : IService(id), gstreamer_() {}

void Video::Write(const std::string_view &action) {
    VideoMessage message;
    message.ParseFromArray(action.begin(), action.size());

    switch (message.action()) {
        case VideoMessage::START:
            gstreamer_.Start();
            break;
        case VideoMessage::STOP:
            gstreamer_.Stop();
            break;
    }

}

Video::~Video() {
    gstreamer_.Stop();
}

void Video::ConnectionLost() {
    gstreamer_.Stop();
}
