//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_COUTPACKET_HPP
#define TICTACTOE_SERVER_COUTPACKET_HPP

#include <cstring>
#include "CPacket.hpp"
#include "ITcpUser.hpp"

class COutPacket : protected CPacket {
public:
    COutPacket(ePackets type, const uint8_t* body, uint16_t sizeBody) {
        write(type, body, sizeBody);
    }
    void write(ePackets type, const uint8_t* body, uint16_t sizeBody) noexcept {
        if (type == ePackets::NONE || (body == nullptr && sizeBody != 0))
            return;
        m_head = {type, sizeBody};
        m_body = std::make_unique<uint8_t[]>(sizeBody);
        memcpy(m_body.get(), body, sizeBody);
    }

    void send(const ITcpUser& user) const {
        auto size = sizeof(m_head) + m_head.m_bodySize;
        std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(size);
        memcpy(data.get(), &m_head, sizeof(m_head));
        memcpy(data.get() + sizeof(m_head), m_body.get(), m_head.m_bodySize);
        user.send(data.get(), size);
    }
};

#endif //TICTACTOE_SERVER_COUTPACKET_HPP
