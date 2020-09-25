#pragma once

#include <list>
#include <memory>

#include "gamestate.h"
#include "Random.h"
#include "entities.h"
#include "textureHolder.h"

class PlayState : public GameState {
public:
    PlayState(Game * pGame);
    virtual ~PlayState() {}

    virtual void init() override;
    virtual void handleEvents(sf::Event event) override;
    virtual void update(sf::Time dt = sf::Time::Zero) override;
    virtual void render() override;

private:

    enum entityTypes {
        spaceship,
        bullet,
        animation,
        asteroid_big,
        asteroid_medium,
        asteroid_small,
        Count
    };

    void createEntity(entityTypes type, const sf::Vector2f & position = sf::Vector2f(0.f, 0.f), const sf::Vector2f & velocity = sf::Vector2f(0.f, 0.f));
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);

    TextureHolder mTextureHolder;

    sf::Sprite mBackgroundSprite;
    std::list<std::unique_ptr<Entity>> mObjects;
    std::unique_ptr<Player> mPlayer;

    float mVelocity = 120.0f;

    bool mIsSpacePressed = false;

    Random<std::mt19937> mEngine;

    sf::Text mPlayerScore;
    sf::Text mPlayerLives;
};
