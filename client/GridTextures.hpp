//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_GRIDTEXTURES_HPP
#define TICTACTOE_CLIENT_GRIDTEXTURES_HPP

#include "GridCell.hpp"

class GridTextures {
    sf::Vector2f texture_size;
    EmptyCell empty;
    XCell     x;
    OCell     o;
protected:
    GridCell* getEmptyTexture() { return &empty; }
    GridCell* getXTexture() { return &x; }
    GridCell* getOTexture() { return &o; }

    template <typename T>
    const auto& getSize() const {
        static sf::Vector2<T> size{ static_cast<T>(texture_size.x), static_cast<T>(texture_size.y) };
        return size;
    }

    GridTextures(float width, float height)
            : texture_size { width, height }, empty { texture_size }, x { empty.getSize() }, o { empty.getSize() } {
        if (width < 1 || height < 1)
            texture_size = empty.getSize();
    }

public:
    virtual ~GridTextures() = 0;
};
inline GridTextures::~GridTextures() = default;

#endif //TICTACTOE_CLIENT_GRIDTEXTURES_HPP
