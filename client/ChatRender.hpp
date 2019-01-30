#include <utility>

//
// Created by root on 19.12.18.
//

#ifndef PROJECT_CHATRENDER_HPP
#define PROJECT_CHATRENDER_HPP

#include <cstring>
#include <iostream>
#include <sstream>
#include "RenderLayer.hpp"
#include "resources.hpp"

enum eKeysCodes { eEnter = 13, eBackspace = 8, eDel = 127, eEsc = 27};
const char* WelcomeMessage = "Enter your message...";
using entered_handler = std::function<void(const std::basic_string<sf::Uint8>&)>;

class ChatRender : public RenderLayer {
    std::pair<sf::String, sf::String> gamerName{"X", "O"};
    sf::String buffer, messageBuffer;
    sf::Text chat, message;
    sf::View viewChat, viewMessage;
    sf::Font font;
    sf::RectangleShape entryField;
    sf::RectangleShape chatField;
    float initialViewCenterValue;
    float lineSpacing;
    float endViewPosition;
    entered_handler send{nullptr};

    void initEntryField() {
        entryField.setSize(sf::Vector2f(getSize().x / 2, chat.getCharacterSize() + lineSpacing));
        entryField.setPosition(0, getSize().y - chat.getCharacterSize() - lineSpacing);
        entryField.setFillColor(sf::Color(71, 74, 81, 250));
    }

    void initChatField() {
        chatField.setSize(sf::Vector2f(getSize().x / 2, getSize().y));
        chatField.setPosition(0, 0);
        chatField.setFillColor(sf::Color(71, 74, 81, 180));
    }

    void initChat(const sf::Font &font) {
        chat.setFont(font);
        chat.setFillColor(sf::Color::White);
        chat.setCharacterSize(25);
        chat.setPosition(0, 0);
    }

    void setMessageText(const sf::String &str) {
        message.setString(str);
    }

    void setChatText(const sf::String &str) {
        chat.setString(str);
    }

    void initMessage(const sf::Font& font) {
        message.setFont(font);
        message.setFillColor(sf::Color::White);
        message.setCharacterSize(25);
        message.setPosition(0, entryField.getPosition().y);
    }

    void draw(sf::RenderTarget& render) const final {
        render.setView(viewChat);
        render.draw(chatField);
        render.draw(chat);
        render.setView(viewMessage);
        render.draw(entryField);
        render.draw(message);
    }

    void checkDownBorder(const float& position) {
        if (position == chat.getLocalBounds().height) {
            if (position + lineSpacing > getSize().y - chat.getCharacterSize() - lineSpacing) {
                endViewPosition = (viewChat.getCenter().y > endViewPosition) ? viewChat.getCenter().y : endViewPosition;
                viewChat.setCenter(sf::Vector2f(viewChat.getCenter().x, viewChat.getCenter().y + lineSpacing));
                chatField.setSize(
                        sf::Vector2f(getSize().x / 2, chatField.getSize().y + chat.getCharacterSize() + lineSpacing));
            }
        } else if (position == message.getGlobalBounds().height) {
            viewMessage.setCenter(sf::Vector2f(viewMessage.getCenter().x, viewMessage.getCenter().y + lineSpacing));
            entryField.setSize(
                    sf::Vector2f(getSize().x / 2, entryField.getSize().y + message.getCharacterSize() + lineSpacing));
        }
    }

    void checkRightBorder(const float& position) {
        if (position + message.getCharacterSize() > getSize().x / 2) {
            messageBuffer.insert(messageBuffer.getSize() - 1, "\n");
            checkDownBorder(message.getLocalBounds().height);
            checkDownBorder(chat.getLocalBounds().height);
        }
    }

    void resetMessageEntry() {
        entryField.setSize(sf::Vector2f(getSize().x / 2, message.getCharacterSize() + lineSpacing));
        viewMessage.setCenter(sf::Vector2f(getSize().x / 2, getSize().y / 2));
        messageBuffer = WelcomeMessage;
    }

    void goToTheEnd(const float &position) {
        if (position >= endViewPosition)
            return;
        viewChat.setCenter(sf::Vector2f(viewChat.getCenter().x, endViewPosition + lineSpacing));
    }

    bool isLetter(const sf::Uint32& event) const {
        return ((event >= 32 && event <= 255) || (event >= 1040 && event <= 1103));
    }

    void checkWelcomeMessage() {
        if (messageBuffer == WelcomeMessage)
            messageBuffer.clear();
    }

    void keyboardEvent(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            switch (event.text.unicode) {
                case eEnter:
                    if (messageBuffer == WelcomeMessage)
                        return;
                    updateChatBuffer(messageBuffer, gamerName.first);
                    send(messageBuffer.toUtf8());
                    resetMessageEntry();
                    break;
                case eBackspace:
                    if (!messageBuffer.isEmpty()) {
                        messageBuffer.erase(messageBuffer.getSize() - 1);
                    }
                    break;
                case eDel:
                    break;
                default :
                    if (isLetter(event.text.unicode)) {
                        checkWelcomeMessage();
                        goToTheEnd(viewChat.getCenter().y);
                        checkRightBorder(message.getLocalBounds().width);
                        messageBuffer.insert(messageBuffer.getSize(), event.text.unicode);
                    }
            }
            setMessageText(messageBuffer);
            setChatText(buffer);
        }
    }

    void mouseEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseWheelScrolled) {
            auto viewCenter = viewChat.getCenter();
            static float max;
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (event.mouseWheelScroll.delta == 1) {
                    max = (viewCenter.y > max) ? viewCenter.y : max;
                    if (viewCenter.y > initialViewCenterValue) {
                        viewChat.setCenter(sf::Vector2f(viewCenter.x, viewCenter.y - lineSpacing));
                        viewCenter.y--;
                    }
                }
                if (event.mouseWheelScroll.delta == -1) {
                    if (viewCenter.y < max) {
                        viewChat.setCenter(sf::Vector2f(viewCenter.x, viewCenter.y + lineSpacing));
                    }
                }
            }
        }
    }

    void updateChatBuffer(const sf::String &string, const sf::String& player) {
        goToTheEnd(viewChat.getCenter().y);
        checkWelcomeMessage();
        buffer += L'\n' + player + ": " + string;
        checkDownBorder(chat.getLocalBounds().height);
        setChatText(buffer);
    }

public:
    void event(const sf::Event& event) final {
        if (event.type > sf::Event::KeyReleased)
            mouseEvent(event);
        else
            keyboardEvent(event);
    }

    explicit ChatRender(const sf::Vector2u& size, ePlayer player, const std::string& fontFile = resources::fonts::Default)
        : viewChat {sf::FloatRect(0, 0, size.x, size.y)}, viewMessage {sf::FloatRect(0, 0, size.x, size.y)},
        RenderLayer(size), messageBuffer { WelcomeMessage } {

        if (player != ePlayer::PLAYER_X)
            std::swap(gamerName.first, gamerName.second);

        if (font.loadFromFile(fontFile)) {
            initEntryField();
            initChatField();
            initChat(font);
            initMessage(font);
            setMessageText(messageBuffer);
            lineSpacing = font.getLineSpacing(chat.getCharacterSize());
            initialViewCenterValue = viewChat.getCenter().y;
        }
    }

    void sendMessage(entered_handler handler) {
       send = std::move(handler);
    }

    void addChat(const sf::String &string) {
        updateChatBuffer(string, gamerName.second);
    }
};

#endif //PROJECT_CHATRENDER_HPP