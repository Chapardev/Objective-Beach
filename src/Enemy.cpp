#include "Enemy.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Enemy::Enemy(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed, float p_value, bool p_axis) 
    : Entity { p_window, p_texture, p_speed }, m_axis { p_axis }
{
    m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f);

    if (p_axis)
    {
        m_sprite.setPosition(p_value + m_sprite.getOrigin().x, m_sprite.getPosition().y + m_sprite.getOrigin().y);
    }
    else
    {
        m_sprite.setPosition(m_sprite.getPosition().x + m_sprite.getOrigin().x, p_value + m_sprite.getOrigin().y);
    }
}

const sf::Texture *Enemy::getTexture() const
{
    return m_sprite.getTexture();
}

bool Enemy::isOutOfBounds() const noexcept
{
    if (m_axis)
    {
        return m_sprite.getPosition().y - m_sprite.getOrigin().y > m_window.getSize().y;
    }

    return m_sprite.getPosition().x - m_sprite.getOrigin().x > m_window.getSize().x;
}

void Enemy::update(float p_dt)
{
    if (!this->isOutOfBounds())
    {
        if (m_axis)
        {
            m_sprite.move(0, m_speed * p_dt);
        }
        else
        {
            m_sprite.move(m_speed * p_dt, 0);
        }

        m_sprite.rotate(m_speed * p_dt);
    }
}
