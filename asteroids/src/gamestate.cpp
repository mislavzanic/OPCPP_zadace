#include "gamestate.h"
#include "game.h"


WelcomeState::WelcomeState(Game * pGame) : GameState(pGame) {
    mWelcome.setFont(mpGame->mFont);
}

void WelcomeState::init() {}

void WelcomeState::handleEvents(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        mIsKeyPressed = true;
    } else if (event.type == sf::Event::KeyReleased) {
        mIsKeyPressed = false;
    }
}

void WelcomeState::update(sf::Time dt) {
    if (mIsKeyPressed) mpGame->setState(GameState::Playing);
}

void WelcomeState::render() {
    sf::RenderWindow * window = mpGame->getWindow();
    mWelcome.setString("Asteroid clone by M.Zanic");
    mWelcome.setOrigin(mWelcome.getLocalBounds().width / 2, mWelcome.getLocalBounds().height / 2);
    mWelcome.setPosition(1280 / 2, 1024 / 4);
    window->draw(mWelcome);
    mWelcome.setString("Press any key to continue");
    mWelcome.setOrigin(mWelcome.getLocalBounds().width / 2, mWelcome.getLocalBounds().height / 2);
    mWelcome.setScale(0.8f, 0.8f);
    mWelcome.setPosition(1280 / 2, (1024 / 4) + 75);
    window->draw(mWelcome);
}


ScoreState::ScoreState(Game * pGame) : GameState(pGame) {
    mText.setFont(mpGame->mFont);
    init();
}

void ScoreState::init() {
    mScore = mpGame->mScore;
    mIsEscPressed = false;
    mIsReturnPressed = false;
}

void ScoreState::handleEvents(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        handlePlayerInput(event.key.code, true);
    } else if (event.type == sf::Event::KeyReleased) {
        handlePlayerInput(event.key.code, false);
    }
}

void ScoreState::update(sf::Time dt) {
    if (mIsReturnPressed) {
        mpGame->setState(GameState::Playing);
        mpGame->mpCurrState->init();
    }
    if (mIsEscPressed) mpGame->getWindow()->close();
}

void ScoreState::render() {
    sf::RenderWindow * window = mpGame->getWindow();
    mText.setString("Score: " + std::to_string(mScore));
    mText.setOrigin(mText.getLocalBounds().width / 2, mText.getLocalBounds().height / 2);
    mText.setPosition(1280 / 2, 1024 / 2);
    window->draw(mText);
    mText.setString("Enter to continue, Esc to exit");
    mText.setOrigin(mText.getLocalBounds().width / 2, mText.getLocalBounds().height / 2);
    mText.setPosition(1280 / 2, (1024 / 2) + 75);
    window->draw(mText);
}

void ScoreState::handlePlayerInput(sf::Keyboard::Key code, bool isPressed) {
    if (code == sf::Keyboard::Key::Return) {
        mIsReturnPressed = isPressed;
    }
    if (code == sf::Keyboard::Key::Escape) {
        mIsEscPressed = isPressed;
    }
}