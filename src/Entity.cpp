#include "Entity.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Entity::Entity(const sf::RenderWindow &p_window, const sf::Texture &p_texture, float p_speed) 
    : m_window { p_window }, m_sprite { p_texture }, m_speed { p_speed }
{
    
}

sf::FloatRect Entity::getGlobalBounds() const noexcept
{
    return m_sprite.getGlobalBounds();
}

void Entity::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    p_target.draw(m_sprite, p_states);
}
