#include "entities.h"
#include "Random.h"

// Dopuniti po potrebi.

Entity::Entity(sf::Texture const & texture){
    mSprite.setTexture(texture);
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width/2, lb.height/2);
    // mSprite.setOrigin(0, 0);
    mVelocity.x=mVelocity.y = 30.0f; 
    isAlive = true;

    mRadius = 1.0*std::min(lb.width/2,lb.height/2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
}

bool Entity::checkCollision(Entity & entity) {
    sf::Vector2f pos1 = getPosition();
    sf::Vector2f pos2 = entity.getPosition();
    float norm2 = std::sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
    if (getRadius() + entity.getRadius() > norm2) return true;
    return false;
}

void Entity::update(sf::Time dt){
    auto pos = getPosition();
    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);
}

Animation::Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel) :
  Entity(texture), mRows(rows), mCols(cols), mWidth(width/cols), mHeight(height/rows), mSpeed(vel),
  mCurrentCol(0.0f),  mCurrentRow(0) {
      mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
      mSprite.setOrigin(mWidth / 2, mHeight / 2);
  }

void Animation::update(sf::Time dt){
    if(!isAlive) return;
    mCurrentCol += mSpeed;
    if(static_cast<int>(mCurrentCol) == mCols){
        mCurrentCol = 0.0f;
        ++mCurrentRow;
    }
    if(mCurrentRow == mRows){
        mCurrentCol = 0.0f;
        mCurrentRow = 0;
        isAlive = false;
        return;
    }

    int i = mCurrentCol;
    int j = mCurrentRow; 
    mSprite.setTextureRect(sf::IntRect(i*mWidth, j*mHeight, mWidth, mHeight));
}

Asteroid::Asteroid(sf::Texture const & texture, const sf::Vector2f & position, const sf::Vector2f & velocity,unsigned int type) : Entity(texture) { 
    mType = type;
    mVelocity = velocity;
    setPosition(position);
}

void Asteroid::update(sf::Time dt) {
    auto pos = getPosition();

    if      (pos.x <= 0)      pos.x = 1280;
    else if (pos.x >= 1280)   pos.x = 0;
    if      (pos.y <= 0)      pos.y = 1024;
    else if (pos.y >= 1024)   pos.y = 0;

    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);

    Random<std::mt19937> engine;
    rotate(engine.getFloatInRange(40.f, 50.f) * dt.asSeconds());
}

Player::Player(sf::Texture const & texture, const sf::Vector2f & position) : Entity(texture) {
    mVelocity = sf::Vector2f(0.f, 0.f);
    setPosition(position);
}

void Player::update(sf::Time dt) {
    auto pos = getPosition();

    if(mIsMovingUp) {
        sf::Vector2f vel = sf::Vector2f(6.f, 0.f);

        sf::Transform rotation;
        rotation.rotate(getRotation() - 90.f);

        vel = rotation.transformPoint(vel);
        float norm2 = mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y;
        if (norm2 < 2.f * 240.f * 240.f)
            mVelocity = sf::Vector2f(mVelocity.x + vel.x, mVelocity.y + vel.y);
    }
    if(mIsMovingDown) {
        float norm2 = (mVelocity.x - 0.05f * mVelocity.x) * (mVelocity.x - 0.05f * mVelocity.x) + (mVelocity.y - 0.05f * mVelocity.y) * (mVelocity.y - 0.05f * mVelocity.y);
        if (norm2 > 0) {
            mVelocity = sf::Vector2f(mVelocity.x - 0.05f * mVelocity.x, mVelocity.y - 0.05f * mVelocity.y);
        }
    }
    if(mIsMovingLeft) {
        rotateLeft(50.f);
        sf::Transform rotation;
        rotation.rotate(-50.f * dt.asSeconds());
        mVelocity = rotation.transformPoint(mVelocity);
    }
    if(mIsMovingRight) {
        rotateRight(50.f);
        sf::Transform rotation;
        rotation.rotate(50.f * dt.asSeconds());
        mVelocity = rotation.transformPoint(mVelocity);
    }

    if      (pos.x <= 0)      pos.x = 1280;
    else if (pos.x >= 1280)   pos.x = 0;
    if      (pos.y <= 0)      pos.y = 1024;
    else if (pos.y >= 1024)   pos.y = 0;

    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);

    rotate(mNewRotation * dt.asSeconds());
    mNewRotation = 0.f;
}

void Player::handlePlayerInput(sf::Keyboard::Key code, bool isPressed) {
    if (code == sf::Keyboard::Key::Up) {
        mIsMovingUp = isPressed;
    }
    if (code == sf::Keyboard::Key::Down) {  
        mIsMovingDown = isPressed;
    }
    if (code == sf::Keyboard::Key::Left) {
        mIsMovingLeft = isPressed;
    }
    if (code == sf::Keyboard::Key::Right) {
        mIsMovingRight = isPressed;            
    }
}

Bullet::Bullet(sf::Texture const & texture) : Entity(texture) {
    mVelocity.x = 800.f;
    mVelocity.y = 0.f;
}

void Bullet::update(sf::Time dt) {
    auto pos = getPosition();
    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);

    if (x < 0.f || x > 1280.f || y < 0.f || y > 1024.f) isAlive = false;
}