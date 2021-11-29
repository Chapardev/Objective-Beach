#ifndef SRC_STATES_HPP
#define SRC_STATES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

template<typename T>
using Dictionary = std::unordered_map<std::string, T>;
using String = const std::string &;
using Key = String;

class State
{
protected:
    void loadTexture(Key p_keyName, String p_fileName);
    void createText(Key p_keyName, String p_string, unsigned int p_size, float p_x, float p_y);
    void createCenteredText(Key p_keyName, String p_string, unsigned int p_size);
    void loadSound(Key p_keyName, String p_fileName);
    void playSound(Key p_keyName);

public:
    State(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);

    virtual void checkEvents(sf::Event &p_event);
    virtual void update() = 0;
    virtual void draw() = 0;

protected:
    sf::RenderWindow &m_window;
    std::stack<std::unique_ptr<State>> &m_states;

    Dictionary<sf::Texture> &m_textures;
    Dictionary<sf::Sprite> &m_sprites;

    sf::Font &m_font;
    Dictionary<sf::Text> &m_texts;

    Dictionary<sf::SoundBuffer> &m_buffers;
    Dictionary<sf::Sound> &m_sounds;
};

#endif // SRC_STATES_HPP
