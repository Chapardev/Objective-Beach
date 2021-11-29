#ifndef SRC_INPUT_STATE_HPP
#define SRC_INPUT_STATE_HPP

#include "State.hpp"

// InputState will ask you to choose your keyboard layout
class InputState : public State
{
public:
    InputState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);
    
    // Write the chosen layout in a file
    static void saveLayout(String p_layout);
    void goToMenu();

    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    sf::RectangleShape m_wasdShape;
    sf::RectangleShape m_zqsdShape;
};

#endif // SRC_INPUT_STATE_HPP
