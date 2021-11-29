#include "Turtle.hpp"

#include "Enemy.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <fstream>

Turtle::Turtle(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed) : Entity { p_window, p_texture, p_speed }
{
    m_healthBar.setSize({ p_window.getSize().x * 1.f, 10.f });
    m_healthBar.setPosition(0, p_window.getSize().y - m_healthBar.getSize().y);
    m_healthBar.setFillColor(sf::Color::Red);

    m_healthBarBg = m_healthBar;
    m_healthBarBg.setFillColor({ 64, 64, 64 });

    std::ifstream ifs { "../assets/data/layout.txt" };
    if (!ifs)
    {
        throw std::string { "Unable to open file" };
    }

    int upKey { };
    int leftKey { };
    ifs >> upKey >> leftKey;

    m_upKey = static_cast<sf::Keyboard::Key>(upKey);
    m_leftKey = static_cast<sf::Keyboard::Key>(leftKey);

    m_sprite.setPosition(
        (m_window.getSize().x - m_sprite.getGlobalBounds().width) / 2.f,  
        (m_window.getSize().y - m_sprite.getGlobalBounds().height) / 2.f
    );
}

sf::Vector2f Turtle::getPosition() const noexcept
{
    return m_sprite.getPosition() - m_sprite.getOrigin();
}

bool Turtle::collidesWith(const Enemy &p_enemy) const noexcept
{
    return this->getGlobalBounds().intersects(p_enemy.getGlobalBounds());
}

void Turtle::handleMovement(float p_dt)
{
    if (sf::Keyboard::isKeyPressed(m_upKey))
    {
        m_sprite.move(0, -m_speed * p_dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_sprite.move(0, m_speed * p_dt);
    }

    if (sf::Keyboard::isKeyPressed(m_leftKey))
    {
        m_sprite.move(-m_speed * p_dt, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_sprite.move(m_speed * p_dt, 0);
    }
}

void Turtle::checkCollisions()
{
    if (m_sprite.getPosition().x < 0)
    {
        m_sprite.setPosition(0, m_sprite.getPosition().y);
    }
    else if (m_sprite.getPosition().x > m_window.getSize().x - m_sprite.getGlobalBounds().width)
    {
        m_sprite.setPosition(m_window.getSize().x - m_sprite.getGlobalBounds().width, m_sprite.getPosition().y);
    }

    if (m_sprite.getPosition().y < 0)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, 0);
    }
    else if (m_sprite.getPosition().y > m_window.getSize().y - m_sprite.getGlobalBounds().height - m_healthBar.getSize().y)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, m_window.getSize().y - m_sprite.getGlobalBounds().height - m_healthBar.getSize().y);
    }
}

void Turtle::update(float p_dt)
{
    m_healthBar.setSize({ (m_lives * m_window.getSize().x) / m_livesMax, m_healthBar.getSize().y * 1.f });

    this->handleMovement(p_dt);
    this->checkCollisions();
}

void Turtle::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    Entity::draw(p_target, p_states);
    p_target.draw(m_healthBarBg, p_states);
    p_target.draw(m_healthBar, p_states);
}
