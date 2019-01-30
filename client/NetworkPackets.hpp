#ifndef PROJECT_NETWORKPACKETS_HPP
#define PROJECT_NETWORKPACKETS_HPP

#include <iostream>

enum class ePackets : uint8_t {
    NONE,                /*incoming-outcoming*/
    CHAT,
    SELECT_CELL,         /*incoming*/
    UPDATE_GRID,         /*outcoming*/
    START_GAME,
    STOP_GAME
};


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

#endif //PROJECT_NETWORKPACKETS_HPP
