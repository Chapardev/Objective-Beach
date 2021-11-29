#include "InputState.hpp"

#include "MainMenuState.hpp"

#include <fstream>

InputState::InputState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds)
    : State { p_window, p_states, p_textures, p_sprites, p_font, p_texts, p_buffers, p_sounds }
{
    m_sprites.emplace("beach", m_textures.at("beach"));

    m_sprites.emplace("wasd", m_textures.at("wasd"));
    m_sprites.at("wasd").setPosition(30, (m_window.getSize().y - m_sprites.at("wasd").getGlobalBounds().height) / 2.f);

    m_sprites.emplace("zqsd", m_textures.at("zqsd"));
    m_sprites.at("zqsd").setPosition(
        m_window.getSize().x -  m_sprites.at("zqsd").getGlobalBounds().width - 30, 
        m_sprites.at("wasd").getPosition().y
    );

    this->createText("choose_layout", "Choose your keyboard layout", 30, 0, 0);
    m_texts.at("choose_layout").setPosition(
        (m_window.getSize().x - m_texts.at("choose_layout").getGlobalBounds().width) / 2.f,
        (m_window.getSize().y - m_texts.at("choose_layout").getGlobalBounds().height) * 0.75f
    );

    m_wasdShape.setSize({ m_sprites.at("wasd").getGlobalBounds().width, m_sprites.at("wasd").getGlobalBounds().height });
    m_wasdShape.setFillColor(sf::Color { 0, 0, 0, 0 });
    m_wasdShape.setOutlineThickness(3.f);
    m_wasdShape.setOutlineColor(sf::Color::White);
    m_wasdShape.setPosition(m_sprites.at("wasd").getPosition());

    m_zqsdShape.setSize({ m_sprites.at("zqsd").getGlobalBounds().width, m_sprites.at("zqsd").getGlobalBounds().height });
    m_zqsdShape.setFillColor(sf::Color { 0, 0, 0, 0 });
    m_zqsdShape.setOutlineThickness(3.f);
    m_zqsdShape.setOutlineColor(sf::Color::White);
    m_zqsdShape.setPosition(m_sprites.at("zqsd").getPosition());

    this->loadSound("changed", "../assets/sfx/changed.wav");
}

void InputState::saveLayout(String p_layout)
{
    std::ofstream ofs { "../assets/data/layout.txt" };
    if (!ofs)
    {
        throw std::string { "Unable to open file" };
    }

    if (p_layout == "wasd")
    {
        ofs << sf::Keyboard::W << ' ' << sf::Keyboard::A;
    }
    else if (p_layout == "zqsd")
    {
        ofs << sf::Keyboard::Z << ' ' << sf::Keyboard::Q;
    }
}

void InputState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        m_window.close();
    }
    else if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        if (m_sprites.at("wasd").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
        {
            this->saveLayout("wasd");
            m_states.push(std::make_unique<MainMenuState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
            m_sounds.at("changed").play();
        }
        else if (m_sprites.at("zqsd").getGlobalBounds().contains(p_event.mouseButton.x, p_event.mouseButton.y))
        {
            this->saveLayout("zqsd");
            m_states.push(std::make_unique<MainMenuState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
            m_sounds.at("changed").play();
        }
    }
}

void InputState::update()
{
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
}

void InputState::draw()
{
    m_window.draw(m_sprites.at("beach"));
    m_window.draw(m_sprites.at("wasd"));
    m_window.draw(m_sprites.at("zqsd"));

    if (m_wasdShape.getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }))
    {
        m_window.draw(m_wasdShape);
    }
    else if (m_zqsdShape.getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) }))
    {
        m_window.draw(m_zqsdShape);
    }

    m_window.draw(m_texts.at("choose_layout"));
    m_window.draw(m_sprites.at("cursor"));
}
