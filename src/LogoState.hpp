#ifndef SRC_LOGO_STATE_HPP
#define SRC_LOGO_STATE_HPP

#include "State.hpp"

class LogoState : public State
{
public:
    LogoState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);
    
    void loadTextures();
    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    const sf::Time m_duration { sf::seconds(2) };
    bool m_layoutSelected { false };
    sf::Clock m_clock;
};

#endif // SRC_LOGO_STATE_HPP
