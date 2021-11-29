#ifndef SRC_GAME_STATE_HPP
#define SRC_GAME_STATE_HPP

#include "State.hpp"

#include "Enemy.hpp"
#include "Turtle.hpp"

#include <array>
#include <vector>

using Line = std::array<sf::Vertex, 2>;

class GameState : public State
{
public:
    GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, Dictionary<sf::Texture> &p_textures, 
        Dictionary<sf::Sprite> &p_sprites, sf::Font &p_font, Dictionary<sf::Text> &p_texts, Dictionary<sf::SoundBuffer> &p_buffers,
        Dictionary<sf::Sound> &p_sounds);
    
    void addEnemy();
    void reduceTime(float p_divider, String p_newStatusText);
    void updateTime();
    void removeUselessEnemies();
    void updateHook();

    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    const sf::Time m_timeMax { sf::seconds(75 + 1) };
    const sf::Time m_showTextTime { sf::seconds(3) };
    const sf::Time m_apparitionTimeMax { sf::seconds(2) };
    const std::vector<std::string> m_texturesName { "bag", "bottle", "duck", "jerrycan", "spray", "cup", "straw", "diaper", "sailor", "net" };

    sf::Time m_apparitionTime { m_apparitionTimeMax };
    Dictionary<sf::Clock> m_clocks;
    float m_dt { 0.f };

    // Hook
    bool m_hookTaken { false };
    bool m_hookTakenOnce { false };
    bool m_enemyHeld { false };
    Line m_line {  };
    sf::RectangleShape m_timeBarBg;
    sf::RectangleShape m_timeBar;

    const sf::Time m_hookDuration { sf::seconds(5) };
    
    static constexpr float s_turtleSpeed { 250.f };
    std::unique_ptr<Turtle> m_turtle;

    std::vector<std::unique_ptr<Enemy>> m_enemies;

    sf::Music m_music;
};

#endif // SRC_GAME_STATE_HPP
