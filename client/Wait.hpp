//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_WAIT_HPP
#define TICTACTOE_CLIENT_WAIT_HPP

#include "RenderLayer.hpp"
#include "StringMenuItem.hpp"
#include "resources.hpp"

class Wait : public RenderLayer {
    sf::Texture background;
    sf::Sprite sprite;
    sf::Font font;
    StringMenuItem waiting;

    void initBackground(const sf::String& image) {
        background.loadFromFile(image);
        background.setSmooth(true);

        sf::Vector2f size(getSize());
        sf::Vector2f textureSize(background.getSize());
        sprite.setTexture(background);
        sprite.scale(size.x / textureSize.x, size.y / textureSize.y);
    }

    void initText(StringMenuItem &button, float xMiddle, float y) {
        button.setColor(sf::Color::White);
        button.getText().setStyle(sf::Text::Regular);
        float x = xMiddle - button.getBounds().width / 2;
        button.updatePosition({x, y});
    }

public:
    explicit Wait(const sf::Vector2u& size, const sf::String& fontName = resources::fonts::Default,
                  const sf::String& backImage = resources::textures::Background)
            : RenderLayer(size), waiting { "Waiting for the second player ", font, 20, true } {
        font.loadFromFile(fontName);
        initBackground(backImage);

        //TODO: автовычисленplayие
        float start = size.y / 2 - 50;//20 - высота текста, 20 - промежуток между пунктами меню
        float middle = size.x / 2;

        waiting.updatePosition({middle - waiting.getBounds().width / 2, start});
        initText(waiting, middle, start + 40);
    }
protected:
    void draw(sf::RenderTarget &render) const override {
        render.draw(sprite);
        render.draw(waiting);
    }
};

#endif //TICTACTOE_CLIENT_WAIT_HPP