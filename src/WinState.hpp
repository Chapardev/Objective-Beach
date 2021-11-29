#ifndef SRC_WIN_STATE_HPP
#define SRC_WIN_STATE_HPP

#include "State.hpp"

class WinState : public State
{
public:
    WinState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);
    
    void createTexts();
    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    static constexpr float m_textMoveMax { 5.f };
    float m_textStartPosY { };
    sf::Clock m_clock;
};

#endif // SRC_WIN_STATE_HPP
