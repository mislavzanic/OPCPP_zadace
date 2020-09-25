#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable {
public:
    Entity(sf::Texture const & texture);

    void setVelocity(sf::Vector2f const &  vel) { mVelocity = vel;}

    sf::Vector2f getVelocity() const { return mVelocity; }
    float getRadius() const { return mRadius; }

    bool checkCollision(Entity & entity);
    virtual void update(sf::Time dt = sf::Time::Zero); 
    virtual ~Entity() {}

    bool isAlive;
    unsigned int mType = 0;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Sprite mSprite;
    sf::Vector2f mVelocity;
    sf::CircleShape mCircle;
    float mRadius;
};

class Animation : public Entity {
public:
    Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel);
    virtual void update(sf::Time dt = sf::Time::Zero)  override; 
private:
    int mRows, mCols, mWidth, mHeight;
    float mSpeed;
    float mCurrentCol;
    int   mCurrentRow;
};

// dopuniti
class Asteroid : public Entity {
public:
    Asteroid(sf::Texture const & texture, const sf::Vector2f & position, const sf::Vector2f & velocity, unsigned int type);

    virtual void update(sf::Time dt = sf::Time::Zero) override;
    virtual ~Asteroid() {}
};


// dopuniti
class Player : public Entity {
public:
    Player(sf::Texture const & texture, const sf::Vector2f & position);

    virtual void update(sf::Time dt = sf::Time::Zero) override;
    virtual ~Player() {}
    
    void rotateLeft(float angle) { mNewRotation -= angle; }
    void rotateRight(float angle) { mNewRotation += angle; }

    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);

    int mLives = 3;
    int mScore = 0;

private:
    float mNewRotation = 0.f;

    bool mIsMovingUp     = false;
    bool mIsMovingDown   = false;
    bool mIsMovingLeft   = false;
    bool mIsMovingRight  = false;
};

class Bullet : public Entity {
public:
    Bullet(sf::Texture const & texture);
    virtual ~Bullet() {}
    virtual void update(sf::Time dt = sf::Time::Zero) override;
};
