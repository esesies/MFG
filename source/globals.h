#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals
{
  const int SCREEN_WIDTH  = 640;
  const int SCREEN_HEIGHT = 480;

  const float MAX_WIDTH = (float)(SCREEN_WIDTH - 32.0f);
  const float MAX_HEIGHT = (float)(SCREEN_HEIGHT - 32.0f);

  const float SPRITE_SCALE = 2.0f;
  const float N_PI = 3.14159265f;
}

namespace sides
{
  enum Side
  {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    NONE
  };

  inline Side GetOppositeSide(Side side)
  {
    return
      side == TOP ? BOTTOM :
      side == BOTTOM ? TOP :
      side == LEFT ? RIGHT :
      side == RIGHT ? LEFT :
      NONE;
  }
}

enum Color
{
  BLUE,
  RED,
  GREEN,
  WHITE
};



enum Direction
{
  LEFT,
  RIGHT,
  UP,
  DOWN
};

struct Vector2
{
  int x, y;
  Vector2() : x(0), y(0) {}
  Vector2(int x, int y) : x(x), y(y) {}
  Vector2 zero() { return Vector2(0, 0); }
};


#endif
