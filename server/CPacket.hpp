//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_CPACKET_HPP
#define TICTACTOE_SERVER_CPACKET_HPP

#include <iostream>
#include <memory>

enum class eCellType : uint8_t {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
};

enum class ePlayer : uint8_t {
    PLAYER_X,
    PLAYER_O
};

enum class eWinner : uint8_t {
    PLAYER_X,
    PLAYER_O,
    DEAD_HEAT,
    UNKNOWN
};

enum class ePackets : uint8_t {
    NONE,                /*incoming-outcoming*/
    CHAT,
    SELECT_CELL,         /*incoming*/
    UPDATE_GRID,         /*outcoming*/
    START_GAME,
    STOP_GAME
};

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

#endif //TICTACTOE_SERVER_CPACKET_HPP
