#include "player.h"
#include "graphics.h"

namespace player_constants
{
  const float WALK_SPEED = 0.2f;
}

Player::Player()
{

}

Player::Player(Graphics &graphics, float x, float y) :
  AnimatedSprite(graphics, "../content/sprites/MyChar.png", 0, 0, 16, 16, x, y, 100)
{
  graphics.loadImage("../content/sprites/MyChar.png");
  //NOTE: En el constructor, llamamos a setupAnimations y a playAnimation en vez de hacerlo en game.cpp
  this->setupAnimations();
  this->playAnimation("RunLeft");
}

void Player::setupAnimations(void)
{
  this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
  this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
  this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
  this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation)
{

}

void Player::moveLeft(void)
{
  this->_dx = -player_constants::WALK_SPEED;
  this->playAnimation("RunLeft");
  this->_facing = LEFT;
}

void Player::moveRight(void)
{
  this->_dx = player_constants::WALK_SPEED;
  this->playAnimation("RunRight");
  this->_facing = RIGHT;
}

void Player::moveUp(void)
{
  this->_dy = -player_constants::WALK_SPEED;;
  //this->playAnimation("RunUp");
  this->_facing = UP;
}

void Player::moveDown(void)
{
  this->_dy = player_constants::WALK_SPEED;;
  //this->playAnimation("RunUp");
  this->_facing = DOWN;
}

void Player::stopMovingX(void)
{
  this->_dx = 0.0f; 
  this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::stopMovingY(void)
{
  this->_dy = 0.0f;
  //this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::update(float elapsedTime)
{
  //Move by dx
  this->_x += this->_dx * elapsedTime;

  //Check screen bounds
  if (this->_x < 0.0f)
    this->_x = 0.0f;
  if (this->_x > globals::MAX_WIDTH)
    this->_x = globals::MAX_WIDTH;

  //Move by dy
  this->_y += this->_dy * elapsedTime;
  
  //Check screen bounds
  if (this->_y < 0.0f)
    this->_y = 0.0f;
  if (this->_y > globals::MAX_HEIGHT)
    this->_y = globals::MAX_HEIGHT;

  AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics)
{
  AnimatedSprite::draw(graphics, this->_x, this->_y);
  //NOTE: Creo que no hace falta poner el this.
}