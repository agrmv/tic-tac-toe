//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_STRINGMENUITEM_HPP
#define TICTACTOE_CLIENT_STRINGMENUITEM_HPP

#include "MenuItem.hpp"

class StringMenuItem : public MenuItem {
    sf::Text text;

public:
    explicit StringMenuItem(const sf::String &string, const sf::Font &font, uint charSize = 20, bool selectable = false)
            : MenuItem(text, selectable) {
        text.setFont(font);
        text.setCharacterSize(charSize);
        updateString(string);
    }

    void updateString(const sf::String &str) {
        text.setString(str);
    }

    void updatePosition(const sf::Vector2f& pos) override {
        text.setPosition(pos);
    }

    void setColor(sf::Color color) {
        text.setFillColor(color);
    }

    sf::Text& getText() {
        return text;
    }

    sf::FloatRect getBounds() override {
        return text.getGlobalBounds();
    }
};

#endif //TICTACTOE_CLIENT_STRINGMENUITEM_HPP