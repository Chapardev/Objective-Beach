#ifndef SRC_ENEMY_HPP
#define SRC_ENEMY_HPP

#include "Entity.hpp"

#include <SFML/System/Vector2.hpp>

class Enemy : public Entity
{
public:
    Enemy(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed, float p_value, bool p_axis);

    const sf::Texture *getTexture() const;
    bool isOutOfBounds() const noexcept;
    void update(float p_dt) override;

private:
    // true: x-axis and false: y-axis
    bool m_axis;
};

#endif // SRC_ENEMY_HPP
