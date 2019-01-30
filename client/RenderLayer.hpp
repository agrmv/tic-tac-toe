#ifndef PROJECT_RENDERBLOCK_HPP
#define PROJECT_RENDERBLOCK_HPP

#include <SFML/Graphics.hpp>

class RenderLayer : public sf::Drawable {
protected:
    mutable sf::RenderTexture m_texture;
    sf::Sprite m_sprite;
    sf::Color m_background;

    explicit RenderLayer(const sf::Vector2u& size, sf::Color color = sf::Color::Transparent) : m_texture {}, m_background { color } {
        m_texture.create(size.x, size.y);
        m_sprite.setTexture(m_texture.getTexture());
    }

    virtual void draw(sf::RenderTarget& render) const = 0;

    template <typename T>
    sf::Vector2f calculateRelativePosition(const T& pos) const {
        return calculateRelativePosition(pos.x, pos.y);
    }

    sf::Vector2f calculateRelativePosition(float x, float y) const {
        return sf::Vector2f{x - m_sprite.getPosition().x,
                            y - m_sprite.getPosition().y};
    }

    bool containedMouse(const sf::Vector2f& position) {
        return position.x >= 0 && position.y >= 0
               && position.x <= m_texture.getSize().x
               && position.y <= m_texture.getSize().y;
    }

public:
    auto getSize() const {
        return m_texture.getSize();
    }

    void setPosition(float x, float y) {
        m_sprite.setPosition(x, y);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const final {
        m_texture.clear(m_background);
        draw(m_texture);
        m_texture.display();
        target.draw(m_sprite);
    }

    virtual void event(const sf::Event& event) {}
};

#endif //PROJECT_RENDERBLOCK_HPP