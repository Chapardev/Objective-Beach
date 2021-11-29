#ifndef SRC_TURTLE_HPP
#define SRC_TURTLE_HPP

#include "Entity.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Enemy;

class Turtle : public Entity
{
public:
    Turtle(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed);

    sf::Vector2f getPosition() const noexcept;
    bool collidesWith(const Enemy &p_enemy) const noexcept;
    
    bool isAlive() const noexcept { return m_lives > 0; }
    void takeDamage(int p_damage) { m_lives -= p_damage; }

    void handleMovement(float p_dt);
    void checkCollisions();
    
    void update(float p_dt) override;
    void draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const override;

private:
    sf::Keyboard::Key m_upKey {  };
    sf::Keyboard::Key m_leftKey {  };

    static constexpr float m_livesMax { 15.f };
    float m_lives { m_livesMax };

    sf::RectangleShape m_healthBar;
    sf::RectangleShape m_healthBarBg;
};

#endif // SRC_TURTLE_HPP
