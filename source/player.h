#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"

class Graphics;


//TODO: Puede que no sea una buena idea que Player herede de AnimatedSprite.
// Me parece que en reddit dijeron que no tenía mucho sentido.
class Player : public AnimatedSprite
{
public:
  Player();
  Player(Graphics &graphics, Vector2 spawnPoint);
  void draw(Graphics &graphics);
  void drawRect(Graphics &graphics);
  void update(float elapsedTime);

  /* void moveLeft
   * Move the player left by -_dx
   */
  void moveLeft(void);

  /* void moveRight
   * Move the player right by +_dx
   */
  void moveRight(void);

  /* void moveUp
  * Move the player up by -_dy
  */
  void moveUp(void);

  /* void moveDown
  * Move the player down by +_dy
  */
  void moveDown(void);
  
  /* void stopMovingX
   * Stops moving the player in the X direction
   */
  void stopMovingX(void);

  /* void stopMovingY
  * Stops moving the player in the Y direction
  */
  void stopMovingY(void);

  /* void jump
  *  Starts jump
  */
  void jump(void);

  virtual void animationDone(std::string currentAnimation);
  virtual void setupAnimations(void);

  void HandleTileCollisions(std::vector<Rectangle>& others);
  void HandleSlopeCollision(std::vector<Slope>& others);

  const float getX() const;
  const float getY() const;

private:
  float _dx, _dy; //represents the change in the x and y positions during the frame.

  Direction _facing;

  bool _grounded;

};


#endif //PLAYER_H
