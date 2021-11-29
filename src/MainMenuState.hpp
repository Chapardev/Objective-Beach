#ifndef SRC_MAIN_MENU_STATE_HPP
#define SRC_MAIN_MENU_STATE_HPP

#include "State.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);

    void readFile();
    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    static constexpr float m_titleMoveMax { 5.f };
    float m_titleStartPosY { };
    sf::Clock m_clock;
};

#endif // SRC_MAIN_MENU_STATE_HPP
