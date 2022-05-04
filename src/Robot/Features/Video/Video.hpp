#ifndef ROBOT_VIDEO_HPP
#define ROBOT_VIDEO_HPP

#include "./Gstreamer.hpp"

#include <TcpSession/TcpSession.hpp>

namespace app {

    class Video final : public lib::network::IService {
    public:

        Video(const Video&) = delete;
        Video(Video&&) = delete;
        Video& operator=(const Video&) = delete;
        Video& operator=(Video&&) = delete;

        explicit Video(ssize_t id);
        ~Video();
        Response Write(const std::string_view &action) final;

    private:
        void StartVideo(const std::string&);
        static pid_t KillStream(pid_t process);

        pid_t childPid_{};
    };
}

#endif
