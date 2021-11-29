#include "State.hpp"

void State::loadTexture(Key p_keyName, String p_fileName)
{
    if (!m_textures[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load \"" + p_keyName + "\" texture (path: \"" + p_fileName + "\")";
    }
}

void State::createText(Key p_keyName, String p_string, unsigned int p_size, float p_x, float p_y)
{
    m_texts.emplace(p_keyName, sf::Text { p_string, m_font, p_size });
    m_texts.at(p_keyName).setOutlineThickness(2.f);
    m_texts.at(p_keyName).setOutlineColor(sf::Color::Black);
    m_texts.at(p_keyName).setPosition(p_x, p_y);
}

void State::createCenteredText(Key p_keyName, String p_string, unsigned int p_size)
{
    this->createText(p_keyName, p_string, p_size, 0.f, 0.f);
    m_texts.at(p_keyName).setPosition(
        (m_window.getSize().x - m_texts.at(p_keyName).getGlobalBounds().width) / 2.f, 
        (m_window.getSize().y - m_texts.at(p_keyName).getCharacterSize()) / 2.f
    );
}

void State::loadSound(Key p_keyName, String p_fileName)
{
    if (!m_buffers[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load \"" + p_keyName + "\" texture (path: \"" + p_fileName + "\")";
    }
    m_sounds.emplace(p_keyName, m_buffers.at(p_keyName));
}

void State::playSound(Key p_keyName)
{
    if (m_sounds.at(p_keyName).getStatus() != sf::Sound::Playing)
    {
        m_sounds.at(p_keyName).play();
    }
}

State::State(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds)
    : m_window { p_window }, m_states { p_states }, m_textures { p_textures }, m_sprites { p_sprites }, m_font { p_font }, m_texts { p_texts },
      m_buffers { p_buffers }, m_sounds { p_sounds }
{
    
}

void State::checkEvents(sf::Event &p_event)
{

}
