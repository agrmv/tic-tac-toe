//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_CINPACKET_HPP
#define TICTACTOE_CLIENT_CINPACKET_HPP

#include "CPacket.hpp"
#include "ITcpUser.hpp"

class CInPacket : protected CPacket {
public:
    auto getPacketType() const {
        return m_head.m_type;
    }

    auto getBodySize() const {
        return m_head.m_bodySize;
    }

    const uint8_t* getBody() const {
        return m_body.get();
    }

    bool readHead(const ITcpUser& user) {
        return user.recv(&m_head, sizeof(m_head)) == sizeof(m_head);
    }

    bool readBody(const ITcpUser& user) {
        if (getBodySize() == 0)
            return true;
        m_body = std::make_unique<uint8_t[]>(getBodySize());
        return user.recv(m_body.get(), getBodySize()) == getBodySize();
    }

    bool read(const ITcpUser& user) {
        return readHead(user) && readBody(user);
    }

    void clear() {
        m_head = { ePackets::NONE, 0 };
        m_body.reset();
    }
};

#endif //TICTACTOE_CLIENT_CINPACKET_HPP
