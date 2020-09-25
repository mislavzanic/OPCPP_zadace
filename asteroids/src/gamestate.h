#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:
    GameState(Game * pGame) : mpGame(pGame) {}
    virtual ~GameState() {}
    // Sva moguća stanja igre.
        enum State{
            Welcome,
            Playing,
            Score,
            SIZE
        };
    // Metode koje moraju osigurati sva stanja igre.
    virtual void init() = 0;
    virtual void handleEvents(sf::Event event) = 0;
    virtual void update(sf::Time dt = sf::Time::Zero) = 0;
    virtual void render() = 0;

protected:
    Game * const mpGame;
};


class WelcomeState : public GameState{
public:
    WelcomeState(Game * pGame);
    virtual ~WelcomeState() {}

    virtual void init() override;
    virtual void handleEvents(sf::Event event) override;
    virtual void update(sf::Time dt = sf::Time::Zero) override;
    virtual void render() override;
private:
    sf::Text mWelcome;
    bool mIsKeyPressed = false;
};

class ScoreState : public GameState{
public:
    ScoreState(Game * pGame);
    virtual ~ScoreState() {}

    virtual void init() override;
    virtual void handleEvents(sf::Event event) override;
    virtual void update(sf::Time dt = sf::Time::Zero) override;
    virtual void render() override;

private:
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);

    sf::Text mText;
    int mScore = 0;
    bool mIsReturnPressed = false;
    bool mIsEscPressed = false;
};


