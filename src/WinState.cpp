#include "WinState.hpp"

#include <cmath>

WinState::WinState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds)
    : State { p_window, p_states, p_textures, p_sprites, p_font, p_texts, p_buffers, p_sounds }
{
    if (m_sprites.find("turtle") == m_sprites.end())
    {
        m_sprites.emplace("turtle", m_textures.at("turtle"));
        m_sprites.at("turtle").setPosition(
            (m_window.getSize().x - m_sprites.at("turtle").getGlobalBounds().width) / 2.f,  
            (m_window.getSize().y - m_sprites.at("turtle").getGlobalBounds().height) / 2.f
        );

        this->createTexts();
        this->loadSound("win", "../assets/sfx/win.wav");
    }

    m_textStartPosY = (m_window.getSize().y - m_texts.at("congratulations").getGlobalBounds().height) / 4.f;
    m_texts.at("congratulations").setPosition(
        (m_window.getSize().x - m_texts.at("congratulations").getGlobalBounds().width) / 2.f - m_textMoveMax * 2, 
        m_textStartPosY
    );

    this->playSound("win");
}

void WinState::createTexts()
{
    this->createText("congratulations", "Congratulations!", 60, 0, 0);
    m_texts.at("congratulations").setPosition(
        (m_window.getSize().x - m_texts.at("congratulations").getGlobalBounds().width) / 2.f,  
        (m_window.getSize().y - m_texts.at("congratulations").getGlobalBounds().height) / 4.f
    );

    this->createText("subtext", "Caroline will now be able to perpetuate", 30, 0, 0);
    m_texts.at("subtext").setPosition(
        (m_window.getSize().x - m_texts.at("subtext").getGlobalBounds().width) / 2.f,  
        (m_window.getSize().y - m_texts.at("subtext").getGlobalBounds().height) * 0.75f
    );

    this->createText("subtext2", "her species!", 30, 0, 0);
    m_texts.at("subtext2").setPosition(
        (m_window.getSize().x - m_texts.at("subtext2").getGlobalBounds().width) / 2.f,  
        m_texts.at("subtext").getPosition().y + m_texts.at("subtext2").getGlobalBounds().height + 10
    );

    this->createText("menu", "Return to the menu", 30, 0, 0);
    m_texts.at("menu").setPosition(
        (m_window.getSize().x - m_texts.at("menu").getGlobalBounds().width) / 2.f,  
        m_texts.at("subtext2").getPosition().y + m_texts.at("menu").getGlobalBounds().height + 50
    );
}

void WinState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left &&
        m_texts.at("menu").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
    {
        if (m_sounds.at("win").getStatus() == sf::Sound::Playing)
        {
            m_sounds.at("win").stop();
        }
    
        m_states.pop();
        m_states.pop();
    }
}

void WinState::update()
{
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });

    m_texts.at("congratulations").setPosition(
        m_texts.at("congratulations").getPosition().x,
        m_textStartPosY + m_textMoveMax * std::sin(m_clock.getElapsedTime().asSeconds() * 5.f)
    );

    m_texts.at("menu").setStyle(
        m_texts.at("menu").getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }) ?
        sf::Text::Style::Underlined : sf::Text::Style::Regular
    );
}

void WinState::draw()
{
    m_window.draw(m_sprites.at("beach"));
    m_window.draw(m_sprites.at("turtle"));

    m_window.draw(m_texts.at("congratulations"));
    m_window.draw(m_texts.at("subtext"));
    m_window.draw(m_texts.at("subtext2"));
    m_window.draw(m_texts.at("menu"));

    m_window.draw(m_sprites.at("cursor"));
}
