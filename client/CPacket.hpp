//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_CPACKET_HPP
#define TICTACTOE_CLIENT_CPACKET_HPP

#include <memory>
#include "NetworkPackets.hpp"

#pragma pack(push)
#pragma pack(1)
struct stPacketHeader {
    ePackets m_type;
    uint16_t m_bodySize;
};
#pragma pack(pop)

class CPacket {
protected:
    stPacketHeader m_head{};
    std::unique_ptr<uint8_t[]> m_body;
public:
    virtual ~CPacket() = 0;
};
inline CPacket::~CPacket() = default;

#endif //TICTACTOE_CLIENT_CPACKET_HPP