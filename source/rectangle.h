#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

class Rectangle
{
public:
  Rectangle() {}

  Rectangle(int x, int y, int width, int height) :
    _x(x),
    _y(y),
    _width(width),
    _height(height)
  { }

  const int GetCenterX(void) const { return (this->_x + (this->_width / 2)); }
  const int GetCenterY(void) const { return (this->_y + (this->_height / 2)); }
  const int GetLeft(void) const { return this->_x; }
  const int GetRight(void) const { return (this->_x + this->_width); }
  const int GetTop(void) const { return (this->_y); }
  const int GetBottom(void) const { return (this->_y + this->_height); }
  const int GetWidth(void) const { return this->_width; }
  const int GetHeight(void) const { return this->_height; }
  const int GetSide(sides::Side side) const
  {
    return
      side == sides::LEFT ? this->GetLeft() :
      side == sides::RIGHT ? this->GetRight() :
      side == sides::TOP ? this->GetTop() :
      side == sides::BOTTOM ? this->GetBottom() :
      -1;
  }

  const bool CollidesWith(const Rectangle& other) const
  {
    if (this->GetRight() < other.GetLeft())
      return false;
    if (this->GetTop() > other.GetBottom())
      return false;
    if (this->GetLeft() > other.GetRight())
      return false;
    if (this->GetBottom() < other.GetTop())
      return false;
    return true;
  }

  const bool IsValidRectangle(void) const
  {
    return ((this->_x >= 0) && (this->_y >= 0) && (this->_width >= 0) && (this->_height >= 0) );
  }

private:
  int _x, _y, _width, _height;
};



#endif //RECTANGLE_H