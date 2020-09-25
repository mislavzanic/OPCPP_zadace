#include "game.h"

#include <stdexcept>
#include <memory>
#include <typeinfo>


Game::Game() : mWindow(sf::VideoMode(1280, 1024), "SFML window") {

    mAllStates[GameState::Playing] = new PlayState(this);
    mAllStates[GameState::Welcome] = new WelcomeState(this);
    mAllStates[GameState::Score]   = new ScoreState(this);

    mpCurrState = mAllStates[GameState::Welcome];

    mFont.loadFromFile("neuropol_x_rg.ttf");

    mDtFixed = sf::seconds(1.0f/60.0f);
}

Game::~Game() {
    delete mAllStates[GameState::Playing];
    delete mAllStates[GameState::Welcome];
    delete mAllStates[GameState::Score];
}

void Game::run(){
    // Start the game loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate >= mDtFixed){
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }
        render();
    }

}

void Game::processEvents(){
    // Process events
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        // Close window : exit
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        } else {
            mpCurrState->handleEvents(event);
        }
    }
}

void Game::update(sf::Time dt){
    mpCurrState->update(dt);
}

void Game::render(){
    mWindow.clear();
    mpCurrState->render();
    mWindow.display();
}
