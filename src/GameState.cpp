#include "GameState.hpp"

#include "WinState.hpp"

#include <effolkronium/random.hpp>

#include <fstream>

using Random = effolkronium::random_static;

GameState::GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds) 
    : State { p_window, p_states, p_textures, p_sprites, p_font, p_texts, p_buffers, p_sounds }
{
    if (m_sprites.find("sea") == m_sprites.end())
    {
        m_sprites.emplace("sea", m_textures.at("sea"));
        m_sprites.emplace("cursor", m_textures.at("cursor"));
        m_sprites.emplace("hook", m_textures.at("hook"));
        m_sprites.at("hook").setOrigin(m_sprites.at("hook").getGlobalBounds().width / 2.f, 0);

        this->createText("timer", "00", 30, 0, 0);
        m_texts.at("timer").setPosition(
            (m_window.getSize().x - m_texts.at("timer").getGlobalBounds().width) / 2.f,  
            (m_window.getSize().y - m_texts.at("timer").getGlobalBounds().height) / 4.f
        );

        this->createText("status", "", 30, 0, 0);

        this->loadSound("hurt", "../assets/sfx/hurt.wav");
        this->loadSound("powerup", "../assets/sfx/powerup.wav");
        this->loadSound("net_powerup", "../assets/sfx/net_powerup.wav");
        this->loadSound("lose", "../assets/sfx/lose.wav");
    }

    m_texts.at("status").setString("Survive for 75 seconds!");
    m_texts.at("status").setPosition(
        (m_window.getSize().x - m_texts.at("status").getGlobalBounds().width) / 2.f,  
        (m_window.getSize().y - m_texts.at("status").getGlobalBounds().height) * 0.75f
    );

    if (!m_music.openFromFile("../assets/music/bubbles.wav"))
    {
        throw std::string { "Unable to load music" };
    }
    m_music.setVolume(75);
    m_music.play();

    m_clocks.emplace("timer", sf::Clock { });
    m_clocks.emplace("apparition", sf::Clock { });
    m_clocks.emplace("show_text", sf::Clock { });
    m_clocks.emplace("dt", sf::Clock { });
    m_clocks.emplace("hook", sf::Clock { });

    m_turtle = std::make_unique<Turtle>(m_window, m_textures.at("turtle"), s_turtleSpeed);

    m_timeBarBg.setSize({ m_window.getSize().x * 1.f, 10.f });
    m_timeBarBg.setFillColor({ 64, 64, 64 });
    m_timeBarBg.setPosition(0, p_window.getSize().y - m_timeBarBg.getSize().y * 2);

    m_timeBar = m_timeBarBg;
    m_timeBar.setFillColor(sf::Color::Yellow);

    this->addEnemy();
}

void GameState::addEnemy()
{
    std::string name {
        m_clocks.at("timer").getElapsedTime().asSeconds() > 45 ?
        *Random::get(m_texturesName) : *Random::get(m_texturesName.begin(), m_texturesName.end()-2)
    };

    float speed { };
    if (m_clocks.at("timer").getElapsedTime().asSeconds() > 15)
    {
        speed = Random::get(1, 3) * 75.f;
    }
    else if (m_clocks.at("timer").getElapsedTime().asSeconds() > 30)
    {
        speed = Random::get(1, 3) * 100.f;
    }
    else
    {
        speed = Random::get(1, 3) * 50.f;
    }

    bool axis { m_clocks.at("timer").getElapsedTime().asSeconds() > 30 ? Random::get<bool>() : false };
    m_enemies.emplace_back(
        std::make_unique<Enemy>(
            m_window, m_textures.at(name), speed, axis ? m_turtle->getPosition().x : m_turtle->getPosition().y, axis
        )
    );
}

void GameState::reduceTime(float p_divider, const std::string &p_newStatusText)
{
    m_apparitionTime /= p_divider;
    m_texts.at("status").setString(p_newStatusText);
    m_texts.at("status").setPosition(
        (m_window.getSize().x - m_texts.at("status").getGlobalBounds().width) / 2.f,  
        (m_window.getSize().y - m_texts.at("status").getGlobalBounds().height) * 0.75f
    );
    m_clocks.at("show_text").restart();
}

void GameState::updateTime()
{
    m_dt = m_clocks.at("dt").restart().asSeconds();

    if (m_clocks.at("timer").getElapsedTime().asSeconds() > 15 && m_apparitionTime >= m_apparitionTimeMax)
    {
        this->reduceTime(2, "More garbage!");
    }
    else if (m_clocks.at("timer").getElapsedTime().asSeconds() > 30 && m_apparitionTime >= m_apparitionTimeMax / 2.f)
    {
        this->reduceTime(1.5f, "Look up!!");
    }
    else if (m_clocks.at("timer").getElapsedTime().asSeconds() > 45 && m_texts.at("status").getString() != "Catch the net and the sailor!" &&
        m_texts.at("status").getString() != "Use your hook to stop a garbage!")
    {
        this->reduceTime(1, "Catch the net and the sailor!");
    }

    if (m_clocks.at("apparition").getElapsedTime() > m_apparitionTime)
    {
        this->addEnemy();
        m_clocks.at("apparition").restart();
    }
}

void GameState::removeUselessEnemies()
{
    const auto it { std::find_if(m_enemies.begin(), m_enemies.end(), [this](const auto &enemy) { return m_turtle->collidesWith(*enemy); }) };
    if (it != m_enemies.end())
    {
        if ((*it)->getTexture() == &m_textures.at("sailor"))
        {
            m_hookTaken = true;
            if (!m_hookTakenOnce)
            {
                this->reduceTime(1, "Use your hook to stop a garbage!");
                m_hookTakenOnce = true;
            }

            m_clocks.at("hook").restart();
            m_sounds.at("powerup").play();
        }
        else if ((*it)->getTexture() == &m_textures.at("net"))
        {
            m_enemies.clear();
            m_sounds.at("net_powerup").play();
            return;
        }
        else
        {
            m_turtle->takeDamage(1);
            if (m_turtle->isAlive())
            {
                m_sounds.at("hurt").play();
            }
        }

        m_enemies.erase(it);
    }

    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](const auto &enemy) { return enemy->isOutOfBounds(); }), m_enemies.end());
}

void GameState::updateHook()
{
    m_timeBar.setSize({ 
        (m_hookDuration.asSeconds() - m_clocks.at("hook").getElapsedTime().asSeconds()) * m_window.getSize().x / m_hookDuration.asSeconds(), 
        m_timeBar.getSize().y 
    });

    m_sprites.at("hook").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
    m_line[0].position = { 
        m_sprites.at("hook").getPosition().x + 1.f, 
        m_sprites.at("hook").getPosition().y + m_sprites.at("hook").getGlobalBounds().height 
    };
    m_line[1].position = { m_line[0].position.x, m_window.getSize().y * 1.f };

    const auto it { 
        std::find_if(
            m_enemies.begin(), m_enemies.end(),
            [this](const auto &enemy) { 
                return sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
                enemy->getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) });
            }
        )
    };

    m_enemyHeld = it != m_enemies.end();
    for (auto i { m_enemies.begin() }; i != m_enemies.end(); ++i)
    {
        if (!(m_enemyHeld && i == it))
        {
            (*i)->update(m_dt);
        }
    }
}

void GameState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        m_music.stop();
        m_states.pop();
    }
}

void GameState::update()
{ 
    if (m_clocks.at("timer").getElapsedTime() < m_timeMax)
    {
        this->updateTime();
        m_texts.at("timer").setString(std::to_string(static_cast<int>(m_clocks.at("timer").getElapsedTime().asSeconds())));
        m_texts.at("timer").setPosition(
            (m_window.getSize().x - m_texts.at("timer").getGlobalBounds().width) / 2.f,  
            (m_window.getSize().y - m_texts.at("timer").getGlobalBounds().height) / 4.f
        );

        m_turtle->update(m_dt);
        this->removeUselessEnemies();

        if (!m_turtle->isAlive())
        {
            m_music.stop();
            this->playSound("lose");
            m_states.pop();
            // Game crashes if you don't use return
            return;
        }

        if (m_hookTaken && m_clocks.at("hook").getElapsedTime() < m_hookDuration)
        {
            this->updateHook();
        }
        else if (m_hookTaken && m_clocks.at("hook").getElapsedTime() >= m_hookDuration)
        {
            m_hookTaken = false;
        }
        else
        {
            m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
            std::for_each(m_enemies.begin(), m_enemies.end(), [this](const auto &enemy) { enemy->update(m_dt); });
        }
    }
    else
    {
        m_music.stop();
        m_states.push(std::make_unique<WinState>(m_window, m_states, m_textures, m_sprites, m_font, m_texts, m_buffers, m_sounds));
    }
}

void GameState::draw()
{
    if (m_clocks.at("timer").getElapsedTime() < m_timeMax)
    {
        m_window.draw(m_sprites.at("sea"));
        std::for_each(m_enemies.begin(), m_enemies.end(), [this](const auto &enemy) { m_window.draw(*enemy); });
        m_window.draw(*m_turtle);
        m_window.draw(m_texts.at("timer"));

        if (m_clocks.at("show_text").getElapsedTime() < m_showTextTime)
        {
            m_window.draw(m_texts.at("status"));
        }

        if (m_hookTaken && m_clocks.at("hook").getElapsedTime() < m_hookDuration)
        {
            m_window.draw(m_timeBarBg);
            m_window.draw(m_timeBar);
            m_window.draw(&m_line[0], m_line.size(), sf::Lines);
            m_window.draw(m_sprites.at("hook"));
        }
        else
        {
            m_window.draw(m_sprites.at("cursor"));
        }
    }
}
