#pragma once
#include <SFML/Graphics.hpp>

#include <array>

#include "gamestate.h"
#include "playstate.h"

// Klasu dopuniti po potrebi.

class Game{
public:
    Game();
    ~Game();
    void run();

    sf::RenderWindow * getWindow() { return &mWindow; };
    void setState(GameState::State newState) { mpCurrState = mAllStates[newState]; }

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    sf::RenderWindow mWindow;

    std::array<GameState *, 3> mAllStates;
    GameState * mpCurrState;

    sf::Font mFont;
    sf::Time mDtFixed;

    int mScore = 0;

    friend class PlayState;
    friend class ScoreState;
    friend class WelcomeState;
};
