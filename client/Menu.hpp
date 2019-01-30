//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_MENU_HPP
#define TICTACTOE_CLIENT_MENU_HPP

#include <SFML/Window/Event.hpp>
#include "MenuItem.hpp"
#include "RenderLayer.hpp"
#include "StringMenuItem.hpp"
#include "resources.hpp"

class Menu : public RenderLayer {
    sf::Texture background;
    sf::Sprite sprite;
    sf::Font font;
    StringMenuItem play;
    StringMenuItem exit;
    StringMenuItem winner;

    void initBackground(const sf::String& image) {
        background.loadFromFile(image);
        background.setSmooth(true);

        sf::Vector2f size(getSize());
        sf::Vector2f textureSize(background.getSize());
        sprite.setTexture(background);
        sprite.scale(size.x / textureSize.x, size.y / textureSize.y);
    }

    void initButton(StringMenuItem &button, float xMiddle, float y) {
        button.registerEventCallback([&] {
            button.setColor(sf::Color::Red);
            button.getText().setStyle(sf::Text::Bold);
        }, ItemEventType::MouseEnter);
        button.registerEventCallback([&] {
            button.setColor(sf::Color::White);
            button.getText().setStyle(sf::Text::Regular);
        }, ItemEventType::MouseLeave);

        float x = xMiddle - button.getBounds().width / 2;
        button.updatePosition({x, y});
    }

public:
    explicit Menu(const sf::Vector2u& size, const sf::String& fontName = resources::fonts::Default,
                  const sf::String& backImage = resources::textures::Background)
            : RenderLayer(size), play { "Play", font, 20, true },
              exit { "Exit", font, 20, true }, winner { "", font, 25 } {
        font.loadFromFile(fontName);
        initBackground(backImage);

        //TODO: автовычисление
        float start = size.y / 2 - 50;//20 - высота текста, 20 - промежуток между пунктами меню
        float middle = size.x / 2;

        initButton(play, middle, start + 40);
        initButton(exit, middle, start + 80);
    }

    void event(const sf::Event &event) override {
        if (event.type == sf::Event::MouseMoved) {
            auto mouse = calculateRelativePosition(event.mouseMove);
            if (!containedMouse(mouse))
                return;
            play.mouseMove(mouse.x, mouse.y);
            exit.mouseMove(mouse.x, mouse.y);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            auto mouse = calculateRelativePosition(event.mouseButton);
            if (!containedMouse(mouse))
                return;
            play.mouseClick(mouse.x, mouse.y);
            exit.mouseClick(mouse.x, mouse.y);
        }
    }

    void setWinner(const sf::String& text) {
        winner.updateString(text);

        auto size = getSize();
        float start = size.y / 2 - 50;
        float middle = size.x / 2;
        winner.updatePosition({middle - winner.getBounds().width / 2, start});
    }

    void onPlay(const StringMenuItem::mouse_handler &callback) {
        play.registerEventCallback(callback, ItemEventType::MouseClick);
    }

    void onExit(const StringMenuItem::mouse_handler &callback) {
        exit.registerEventCallback(callback, ItemEventType::MouseClick);
    }
protected:
    void draw(sf::RenderTarget &render) const override {
        render.draw(sprite);
        render.draw(play);
        render.draw(exit);
        render.draw(winner);
    }
};

#endif //TICTACTOE_CLIENT_MENU_HPP