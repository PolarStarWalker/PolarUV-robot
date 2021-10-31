#ifndef ROBOT_PACKET_HPP
#define ROBOT_PACKET_HPP

namespace Server {
    enum RequestType : int8_t {
        Undefined = 0,
        Connection = -1,
        R = 2,
        W = 3,
        RW = 4,
    };

    struct Header {
        Header(RequestType requestType, uint64_t messageLength, uint64_t destinationId) :
                _request(requestType),
                _messageLength(messageLength),
                _destinationId(destinationId) {}

        const uint64_t _destinationId;
        const uint64_t _messageLength;
        const RequestType _request;
    };

    class Packet {
    public:
        Packet(RequestType requestType, uint64_t id, const std::byte *data, uint64_t length) {
            _packetLength = sizeof(Header) + length;
            _packet = new std::byte[_packetLength]{};
            Header header(requestType, length,id);
            std::memcpy(_packet, &header, sizeof(Header));
            std::memcpy(_packet + sizeof(header), data, length);
        }

        inline const std::byte& operator[](size_t index) const{
            return _packet[index];
        }

        inline std::byte* Begin(){
            return _packet;
        }

        inline std::byte* MessageBegin(){
            return _packet + sizeof(Header);
        }

        ~Packet() {
            delete[] _packet;
        };

    private:
        std::byte *_packet;
        size_t _packetLength;
    };
};
#endif