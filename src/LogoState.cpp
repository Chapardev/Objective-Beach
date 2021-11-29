#include "LogoState.hpp"

#include "InputState.hpp"
#include "MainMenuState.hpp"

#include <fstream>

LogoState::LogoState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds)
    : State { p_window, p_states, p_textures, p_sprites, p_font, p_texts, p_buffers, p_sounds }
{
    this->loadTextures();

    std::ifstream ifs { "../assets/data/layout.txt" };
    if (ifs)
    {
        m_layoutSelected = true;
    }
    else
    {
        this->loadTexture("wasd", "../assets/images/wasd.png");
        this->loadTexture("zqsd", "../assets/images/zqsd.png");
    }

    m_sprites.emplace("cursor", m_textures.at("cursor"));
    m_sprites.emplace("seajam", m_textures.at("seajam"));

    this->createText("paratext", "For the", 50, 0, 0);
    m_texts.at("paratext").setPosition(
        (m_window.getSize().x - m_texts.at("paratext").getGlobalBounds().width) / 2.f,
        (m_window.getSize().y - m_texts.at("paratext").getGlobalBounds().height) / 4.f - 20
    );

    m_sprites.at("seajam").setPosition(
        (m_window.getSize().x - m_sprites.at("seajam").getGlobalBounds().width) / 2.f,
        m_texts.at("paratext").getPosition().y + m_texts.at("paratext").getGlobalBounds().height + 20
    );

    this->createText("website", "Check out teamseas.org!", 30, 0, 0);
    m_texts.at("website").setPosition(
        (m_window.getSize().x - m_texts.at("website").getGlobalBounds().width) / 2.f,
        m_sprites.at("seajam").getPosition().y + m_sprites.at("seajam").getGlobalBounds().height + 30
    );
}

void LogoState::loadTextures()
{
    this->loadTexture("bag", "../assets/images/bag.png");
    this->loadTexture("beach", "../assets/images/beach.jpg");
    this->loadTexture("bottle", "../assets/images/bottle.png");
    this->loadTexture("cup", "../assets/images/cup.png");
    this->loadTexture("cursor", "../assets/images/cursor.png");
    this->loadTexture("diaper", "../assets/images/diaper.png");
    this->loadTexture("duck", "../assets/images/duck.png");
    this->loadTexture("hook", "../assets/images/hook.png");
    this->loadTexture("jerrycan", "../assets/images/jerrycan.png");
    this->loadTexture("net", "../assets/images/net.png");
    this->loadTexture("sailor", "../assets/images/sailor.png");
    this->loadTexture("sea", "../assets/images/sea.jpg");
    this->loadTexture("seajam", "../assets/images/seajam.png");
    this->loadTexture("spray", "../assets/images/spray.png");
    this->loadTexture("straw", "../assets/images/straw.png");
    this->loadTexture("turtle", "../assets/images/turtle.png");
}

void LogoState::checkEvents(sf::Event &p_event)
{
    
}

void LogoState::update()
{
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
    
    if (m_clock.getElapsedTime() > m_duration)
    {
        if (!m_layoutSelected)
        {
            m_states.push(std::make_unique<InputState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
        }
        else
        {
            m_states.push(std::make_unique<MainMenuState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
        }
    }
}

void LogoState::draw()
{
    m_window.draw(m_texts.at("paratext"));
    m_window.draw(m_texts.at("website"));

    m_window.draw(m_sprites.at("seajam"));
    m_window.draw(m_sprites.at("cursor"));
}
