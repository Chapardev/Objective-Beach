#include "MainMenuState.hpp"

#include "GameState.hpp"
#include "InputState.hpp"

#include <cmath>
#include <fstream>

MainMenuState::MainMenuState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds) 
    : State { p_window, p_states, p_textures, p_sprites, p_font, p_texts, p_buffers, p_sounds }
{
    if (m_sprites.find("beach") == m_sprites.end())
    {
        m_sprites.emplace("beach", m_textures.at("beach"));
    }
    
    this->createText("title", "Objective Beach!", 60, 0, 0);
    m_titleStartPosY = (m_window.getSize().y - m_texts.at("title").getGlobalBounds().height) / 4.f;
    m_texts.at("title").setPosition(
        (m_window.getSize().x - m_texts.at("title").getGlobalBounds().width) / 2.f - m_titleMoveMax * 2, 
        m_titleStartPosY
    );

    this->createCenteredText("play", "Play", 30);
    this->createText("settings", "", 25, 0, 0);
    this->readFile();

    this->createText("quit", "Quit", 30, 0, 0);
    m_texts.at("quit").setPosition(
        (m_window.getSize().x - m_texts.at("quit").getGlobalBounds().width) / 2.f,
        m_texts.at("settings").getPosition().y + m_texts.at("settings").getGlobalBounds().height + 75
    );

    this->loadSound("play", "../assets/sfx/play.wav");
    m_sounds.at("play").setVolume(75);

    if (m_sounds.find("changed") == m_sounds.end())
    {
        this->loadSound("changed", "../assets/sfx/changed.wav");
    }
}

void MainMenuState::readFile()
{
    std::ifstream ifs { "../assets/data/layout.txt" };
    if (!ifs)
    {
        throw std::string { "Unable to open file" };
    }

    int upKey { };
    int leftKey { };
    ifs >> upKey >> leftKey;

    char upKeyLetter { upKey == sf::Keyboard::W ? 'W' : 'Z' };
    char leftKeyLetter { leftKey == sf::Keyboard::A ? 'A' : 'Q' };

    m_texts.at("settings").setString(std::string { "Change layout (current one: " } + upKeyLetter + leftKeyLetter + "SD)");
    m_texts.at("settings").setPosition(
        (m_window.getSize().x - m_texts.at("settings").getGlobalBounds().width) / 2.f,
        m_texts.at("play").getPosition().y + m_texts.at("play").getGlobalBounds().height + 75
    );
}

void MainMenuState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        if (m_texts.at("play").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
        {
            if (m_sounds.find("lose") != m_sounds.end() && m_sounds.at("lose").getStatus() == sf::Sound::Playing)
            {
                m_sounds.at("lose").stop();
            }

            m_states.push(std::make_unique<GameState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
            this->playSound("play");
        }
        else if (m_texts.at("settings").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
        {
            InputState::saveLayout(*(m_texts.at("settings").getString().end()-5) == 'W' ? "zqsd" : "wasd");
            this->readFile();
            m_sounds.at("changed").play();
        }
        else if (m_texts.at("quit").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
        {
            m_window.close();
        }
    }
}

void MainMenuState::update()
{
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });

    m_texts.at("title").setPosition(
        m_texts.at("title").getPosition().x,
        m_titleStartPosY + m_titleMoveMax * std::sin(m_clock.getElapsedTime().asSeconds() * 5.f)
    );

    m_texts.at("play").setStyle(
        m_texts.at("play").getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }) ?
        sf::Text::Style::Underlined : sf::Text::Style::Regular
    );
    
    m_texts.at("settings").setStyle(
        m_texts.at("settings").getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }) ?
        sf::Text::Style::Underlined : sf::Text::Style::Regular
    );

    m_texts.at("quit").setStyle(
        m_texts.at("quit").getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }) ?
        sf::Text::Style::Underlined : sf::Text::Style::Regular
    );
}

void MainMenuState::draw()
{
    m_window.draw(m_sprites.at("beach"));

    m_window.draw(m_texts.at("title"));
    m_window.draw(m_texts.at("play"));
    m_window.draw(m_texts.at("settings"));
    m_window.draw(m_texts.at("quit"));

    m_window.draw(m_sprites.at("cursor"));
}
