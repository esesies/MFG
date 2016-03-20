#ifndef SLOPE_H
#define SLOPE_H

#include <cmath>
#include "globals.h"
#include "rectangle.h"


class Slope
{
public:
  Slope() {}
  Slope(Vector2 p1, Vector2 p2) :
    _p1(p1),
    _p2(p2)
  {
    if (this->_p2.x - this->_p1.x != 0)
    {
      this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
    }
  }

  const inline float GetSlope(void) const { return this->_slope; }
  const int GetPointLeft(void) const 
  {
    if (this->_p1.x < this->_p2.x)
      return _p1.x;
    return _p2.x;
  }
  const int GetPointRight(void) const
  {
    if (this->_p1.x > this->_p2.x)
      return _p1.x;
    return _p2.x;
  }
  const int GetPointTop(void) const
  {
    if (this->_p1.y < this->_p2.y)
      return _p1.y;
    return _p2.y;
  }
  const int GetPointBottom(void) const
  {
    if (this->_p1.y > this->_p2.y)
      return _p1.y;
    return _p2.y;
  }

  const bool CollidesWith(const Rectangle& other)
  {
    if (other.GetRight() < this->GetPointLeft())
      return false;
    if (other.GetBottom() < this->GetPointTop())
      return false;
    if (other.GetLeft() > this->GetPointRight())
      return false;
    if (other.GetTop() > this->GetPointBottom())
      return false;

    //y=mx+n
    //Calculate n
    int n = this->_p1.y - this->_slope * fabs(this->_p1.x);

    //Which corner has collided
    //int xBot;

    //if (this->_slope < 0)
    //  xBot = other.GetRight();
    //else
    //  xBot = other.GetLeft();

    //We take the CenterX to check the collision
    int xBot = other.GetCenterX();

    int y1 = this->_slope * xBot + n;
    if (other.GetBottom() >= y1)
      return true;
    return false;
  }

  const inline Vector2 GetP1() const { return this->_p1; }
  const inline Vector2 GetP2() const { return this->_p2; }

private:
  Vector2 _p1, _p2;
  float _slope; //Pendiente
};


#endif //SLOPE_H