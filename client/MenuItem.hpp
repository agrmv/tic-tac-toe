//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_MENUITEM_HPP
#define TICTACTOE_CLIENT_MENUITEM_HPP

#include <functional>
#include <SFML/Graphics.hpp>

enum class ItemEventType {
    MouseEnter,
    MouseLeave,
    MouseClick
};

class MenuItem : public sf::Drawable {
public:
    typedef std::function<void(void)> mouse_handler;

    template <typename T>
    explicit MenuItem(T& texture, bool selectable = false)
            : drawable { texture }, selectable { selectable } {
    }

    void setSelectable(bool selectable) {
        this->selectable = selectable;
    }

    void registerEventCallback(const mouse_handler& handler, ItemEventType type) {
        switch (type) {
            case ItemEventType::MouseEnter:
                m_mouseEnter = handler;
                break;
            case ItemEventType::MouseLeave:
                m_mouseLeave = handler;
                break;
            case ItemEventType::MouseClick:
                m_mouseClick = handler;
                break;
        }
    }

    void unregisterEventCallback(ItemEventType type) {
        registerEventCallback(nullptr, type);
    }

    void mouseMove(float x, float y) {
        if (!selectable) return;
        bool contain = contained({x, y});
        if (!mouseIn && contain && m_mouseEnter) {
            mouseIn = true;
            return m_mouseEnter();
        }
        if (mouseIn && !contain && m_mouseLeave) {
            mouseIn = false;
            return m_mouseLeave();
        }
    }

    void mouseClick(float x, float y) {
        if (!selectable || !m_mouseClick) return;
        if (contained({x, y}))
            m_mouseClick();
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
        target.draw(drawable, states);
    }

    ~MenuItem() override = default;

    virtual sf::FloatRect getBounds() = 0;
    virtual void updatePosition(const sf::Vector2f& pos) = 0;

private:
    sf::Drawable &drawable;//почему блять нельзя было Text и Sprite от одного интерфейса наследовать, у них одинаковые методы блять есть
    bool selectable;
    bool mouseIn = false;

    mouse_handler m_mouseLeave { nullptr };//массив хэндлеров? индексация через енум
    mouse_handler m_mouseEnter { nullptr };
    mouse_handler m_mouseClick { nullptr };

    bool contained(const sf::Vector2f& position) {
        return getBounds().contains(position);
    }
};

#endif //TICTACTOE_CLIENT_MENUITEM_HPP
