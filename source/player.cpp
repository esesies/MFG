#include "player.h"
#include "graphics.h"

#include <cmath>

namespace player_constants
{
  const float WALK_SPEED = 0.2f;
  const float JUMP_SPEED = 0.7f;
  const float GRAVITY = 0.002f;
  const float GRAVITY_CAP = 0.8f;
}

Player::Player()
{

}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
  AnimatedSprite(graphics, "../content/sprites/MyChar.png", 
    0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
  _dx(0),
  _dy(0),
  _facing(RIGHT),
  _grounded(false)
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

const float Player::getX() const
{
  return this->_x;
}

const float Player::getY() const
{
  return this->_y;
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

void Player::jump(void)
{
  if (this->_grounded)
  {
    this->_dy = -player_constants::JUMP_SPEED;
    this->_grounded = false;
  }
}


//void HandleTileCollision
//Handle collisions with ALL tiles the player is colliding with
void Player::HandleTileCollisions(std::vector<Rectangle>& others)
{
  for (int i = 0; i < others.size(); i++)
  {
    sides::Side collisionSide = Sprite::GetCollisionSide(others.at(i));
    if (collisionSide != sides::NONE)
    {
      switch (collisionSide)
      {
      case sides::TOP:
        this->_y = others.at(i).GetBottom() + 1;
        this->_dy = 0;
        if (this->_grounded)
        {
          this->_dx = 0; //Stop movement
          this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f;
        }          
        break;
      case sides::BOTTOM:
        this->_y = others.at(i).GetTop() - this->_boundingBox.GetHeight() - 1;
        this->_dy = 0;
        this->_grounded = true;
        break;
      case sides::LEFT:
        this->_x = others.at(i).GetRight() + 1;
        break;
      case sides::RIGHT:
        this->_x = others.at(i).GetLeft() - this->_boundingBox.GetWidth() - 1;
        break;
      }
    }
  }
}

//void HandleSlopeCollision
//Handles collisions with ALL slopes tha player is colliding with
void Player::HandleSlopeCollision(std::vector<Slope>& others)
{
  for (int i = 0; i < others.size(); i++)
  {
    int n = others.at(i).GetP1().y - (others.at(i).GetSlope() * fabs(others.at(i).GetP1().x));

    int centerX = this->_boundingBox.GetCenterX();

    int newY = (others.at(i).GetSlope() * centerX) + n - 8; //TODO

    if (this->_grounded)
    {
      this->_y = newY - this->_boundingBox.GetHeight();
      this->_grounded = true;
    }
  }
}

void Player::update(float elapsedTime)
{
  //Apply gravity
  if (this->_dy <= player_constants::GRAVITY_CAP)
    this->_dy += player_constants::GRAVITY * elapsedTime;

  //Move by dx
  this->_x += this->_dx * elapsedTime;

  //Check screen bounds
  if (this->_x < 0.0f)
    this->_x = 0.0f;
  if (this->_x > globals::MAX_WIDTH)
    this->_x = globals::MAX_WIDTH;

  //Move by dy
  this->_y += this->_dy * elapsedTime;

  AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics)
{
  AnimatedSprite::draw(graphics, this->_x, this->_y);
  //NOTE: Creo que no hace falta poner el this.
}