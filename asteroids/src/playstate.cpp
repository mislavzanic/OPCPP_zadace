#include "playstate.h"
#include "game.h"

PlayState::PlayState(Game * pGame) : GameState(pGame) {
    mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));
    createEntity(spaceship, sf::Vector2f(1280 / 2, 1024 / 2));

    init();

    mPlayerScore.setFont(mpGame->mFont);
    mPlayerLives.setFont(mpGame->mFont);
    mPlayerLives.setPosition(20.f, 20.f);
    mPlayerScore.setPosition(1050.f, 20.f);
}

void PlayState::init() {
    if (!mObjects.empty())
        mObjects.clear();

    if (mPlayer.get() != nullptr)
        mPlayer.release();
    
    createEntity(spaceship, sf::Vector2f(1280 / 2, 1024 / 2));
    createEntity(asteroid_big, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));
    createEntity(asteroid_big, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));
    createEntity(asteroid_big, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));
    createEntity(asteroid_medium, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));
    createEntity(asteroid_medium, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));
    createEntity(asteroid_small, sf::Vector2f(mEngine.getFloatInRange(0.f, 1280.f), mEngine.getFloatInRange(0.f, 1024.f)), sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f)));

    mPlayerLives.setString("Lives: " + std::to_string(mPlayer->mLives));
    mPlayerScore.setString("Score: " + std::to_string(mPlayer->mScore));
}

void PlayState::handleEvents(sf::Event event) {
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        handlePlayerInput(event.key.code, true);
        break;
    case sf::Event::KeyReleased:
        handlePlayerInput(event.key.code, false);
        break;
    default:
        break;
    }
} 

void PlayState::update(sf::Time dt) {
    if (mPlayer->isAlive) { 
        if (mIsSpacePressed) {
            createEntity(entityTypes::bullet);
            mIsSpacePressed = false;
        }
    } 
    if (!mPlayer->isAlive || mObjects.empty()) { 
        mpGame->mScore = mPlayer->mScore;
        mpGame->setState(GameState::Score);
        mpGame->mpCurrState->init();
    }

    if (mPlayer->isAlive) {
        
        // kolizija
        std::list<std::unique_ptr<Entity>>::iterator it = mObjects.begin();
        for (auto & object : mObjects) {
            if (typeid(Asteroid) == typeid(*(object.get()))) {

                if (mPlayer->checkCollision(*object)) {
                    mPlayer->mLives--;
                    mPlayerLives.setString("Lives: " + std::to_string(mPlayer->mLives));

                    if (mPlayer->mLives == 0) mPlayer->isAlive = false;
                    object->isAlive = false;

                    createEntity(entityTypes::animation, sf::Vector2f((object->getPosition().x + mPlayer->getPosition().x) / 2, (object->getPosition().y + mPlayer->getPosition().y) / 2));

                    mPlayer->setPosition(1280 / 2, 1024 / 2);
                    mPlayer->setVelocity(sf::Vector2f(0.f, 0.f));
                    mPlayer->setRotation(0.f);
                }

            } else if (typeid(Bullet) == typeid(*(object.get()))) {
                for(auto & asteroids : mObjects) {
                    if (typeid(Asteroid) != typeid(*(asteroids.get()))) break;
                    
                    if (asteroids->checkCollision(*object) && asteroids->isAlive) {     
                        object->isAlive = false;
                        asteroids->isAlive = false;

                        mPlayer->mScore += asteroids->mType;
                        mPlayerScore.setString("Score: " + std::to_string(mPlayer->mScore));

                        if (asteroids->mType == 10) {
                            sf::Vector2f vel = sf::Vector2f(mEngine.getFloatInRange(-240.f, 240.f), mEngine.getFloatInRange(-240.f, 240.f));
                            createEntity(entityTypes::asteroid_small, sf::Vector2f(asteroids->getPosition().x + 0.1f * vel.x, asteroids->getPosition().y + 0.1f * vel.y), vel);
                            createEntity(entityTypes::asteroid_small, sf::Vector2f(asteroids->getPosition().x - 0.1f * vel.x, asteroids->getPosition().y - 0.1f * vel.y), -vel);
                        }

                        createEntity(entityTypes::animation, sf::Vector2f((object->getPosition().x + asteroids->getPosition().x) / 2, (object->getPosition().y + asteroids->getPosition().y) / 2));
                        break;
                    }           
                }
            }
        }

    
        mPlayer->update(dt);

        while (it != mObjects.end()) {
            if ((*it)->isAlive) {
                (*it)->update(dt);
                ++it;
            } else {
                it = mObjects.erase(it);
            }
        }
    }
}

void PlayState::render() {
    sf::RenderWindow * window = mpGame->getWindow();

    window->draw(mBackgroundSprite);

    for (auto & object : mObjects) window->draw(*object);

    if (mPlayer->isAlive) window->draw(*mPlayer);

    window->draw(mPlayerLives);
    window->draw(mPlayerScore);
}

void PlayState::createEntity(entityTypes type, const sf::Vector2f & position, const sf::Vector2f & velocity) {
    if (type == spaceship) {
        mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player), sf::Vector2f(1280 / 2, 1024 / 2)));

    } else if (type == bullet) { 
        std::unique_ptr<Entity> newBullet;
        newBullet.reset(new Bullet(mTextureHolder.getTexture(Textures::bullet)));

        sf::Transform rotation;
        sf::Vector2f vel = newBullet->getVelocity();

        newBullet->setPosition(mPlayer->getPosition());
        newBullet->setRotation(mPlayer->getRotation() - 90.f);

        rotation.rotate(newBullet->getRotation());
        vel = rotation.transformPoint(vel);
        newBullet->setVelocity(vel);

        mObjects.push_back(std::move(newBullet));

    } else if (type == animation) {
        std::unique_ptr<Entity> explosion;
        explosion.reset(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.25f));
        explosion->setPosition(position);
        mObjects.push_back(std::move(explosion));

    } else { 
        std::unique_ptr<Entity> asteroid;

        if (type == asteroid_big) {
            asteroid.reset(new Asteroid(mTextureHolder.getTexture((Textures::ID)mEngine.getIntInRange(4, 7)), position, velocity, 10));
        } else if (type == asteroid_medium) {
            asteroid.reset(new Asteroid(mTextureHolder.getTexture((Textures::ID)mEngine.getIntInRange(8, 9)), position, velocity, 20));
        } else if (type == asteroid_small) {
            asteroid.reset(new Asteroid(mTextureHolder.getTexture((Textures::ID)mEngine.getIntInRange(10, 11)), position, velocity, 40));
        }
            asteroid->setPosition(position);
            asteroid->setVelocity(velocity);
            mObjects.push_front(std::move(asteroid));
    }
}

void PlayState::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if (code == sf::Keyboard::Key::Space) {
        mIsSpacePressed = isPressed;
    } else { 
        mPlayer->handlePlayerInput(code, isPressed);
    }
}