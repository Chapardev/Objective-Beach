#ifndef SRC_ENTITY_HPP
#define SRC_ENTITY_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderWindow;
}

class Entity : public sf::Drawable
{
public:
    Entity(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed);

    sf::FloatRect getGlobalBounds() const noexcept;

    virtual void update(float p_dt) = 0;
    virtual void draw(sf::RenderTarget &p_window, sf::RenderStates p_states) const override;

protected:
    const sf::RenderWindow &m_window;
    sf::Sprite m_sprite;
    float m_speed;
};

#endif // SRC_ENTITY_HPP
