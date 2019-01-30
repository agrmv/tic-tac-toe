//
// Created by root on 19.12.18.
//

#ifndef PROJECT_GRIDCELL_HPP
#define PROJECT_GRIDCELL_HPP

#include <SFML/Graphics.hpp>
#include "resources.hpp"
namespace rcell = resources::textures::cell;

class GridCell {
    std::unique_ptr<sf::Texture> m_texture;
    sf::Vector2f scales;

protected:
    explicit GridCell(const std::string& file, const sf::Vector2f& size)
            : m_texture { std::make_unique<sf::Texture>() }, scales {1.f, 1.f} {
        m_texture->loadFromFile(file);
        m_texture->setSmooth(true);
        if (size.x > 1 && size.y > 1) {
            auto textureSize{ m_texture->getSize() };
            scales = { size.x / textureSize.x, size.y / textureSize.y };
        }
    }

public:
    GridCell() = delete;

    sf::Vector2f getSize() const {
        return { m_texture->getSize().x * scales.x, m_texture->getSize().y * scales.y };
    }

    virtual sf::Sprite getSprite() const {
        sf::Sprite sprite { *m_texture };
        sprite.setScale(scales);
        return sprite;
    }

    virtual sf::Sprite getSprite(float x, float y) const {
        auto sprite { getSprite() };
        sprite.setPosition(x, y);
        return sprite;
    }
    virtual ~GridCell() = 0;
};
inline GridCell::~GridCell() = default;

class EmptyCell : public GridCell {
public:
    explicit EmptyCell(const sf::Vector2f& size = {0, 0}) : GridCell(rcell::Empty, size) {
    }
};

class XCell : public GridCell {
public:
    explicit XCell(const sf::Vector2f& size = {0, 0}) : GridCell(rcell::X, size) {
    }
};

class OCell : public GridCell {
public:
    explicit OCell(const sf::Vector2f& size = {0, 0}) : GridCell(rcell::O, size) {
    }
};


#endif //PROJECT_GRIDCELL_HPP
