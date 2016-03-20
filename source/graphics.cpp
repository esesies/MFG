#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"


/* Graphics class
*  Holds all information dealing with graphics for the game
*/
Graphics::Graphics()
{
  bool success = true;
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
    SDL_SetWindowTitle(this->_window, "Cavestory");
  }

}

Graphics::~Graphics()
{
  SDL_DestroyWindow(this->_window);
  SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath)
{
  if (this->_spriteSheets.count(filePath) == 0) 
  {
    this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
    if (this->_spriteSheets[filePath] == NULL)
    {
      printf("IMG_Load: %s\n", IMG_GetError());
    }
  }
  return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) 
{
  SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::blitSurface(SDL_Texture * texture, SDL_Rect * sourceRectangle, SDL_Rect * destinationRectangle, const double rot)
{
  SDL_RenderCopyEx(this->_renderer, texture, sourceRectangle, destinationRectangle, rot, NULL, SDL_FLIP_NONE);
}

int Graphics::SetColor(Color color)
{
  int setColor = 0;
  switch (color)
  {
  case BLUE:
  {
    setColor = SDL_SetRenderDrawColor(this->_renderer, 0x00, 0x00, 0xFF, 0xFF);
    break;
  }
  case RED:
  {
    setColor = SDL_SetRenderDrawColor(this->_renderer, 0xFF, 0x00, 0x00, 0xFF);
    break;
  }
  case GREEN:
  {
    setColor = SDL_SetRenderDrawColor(this->_renderer, 0x00, 0xFF, 0x00, 0xFF);
    break;
  }
  case WHITE:
  {
    setColor = SDL_SetRenderDrawColor(this->_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    break;
  }
  default:
    break;
  }
  return setColor;
}

void Graphics::DrawRectangle(const SDL_Rect *destinationRectangle, const Color color)
{
  if (destinationRectangle != NULL)
  {
    int setColor;
    int fillRect = 0;

    setColor = this->SetColor(color);

    fillRect = SDL_RenderFillRect(this->_renderer, destinationRectangle);
    if (setColor == -1)
      int hola1 = 1;
    if (fillRect == -1)
      int hola2 = 1;
  }
}

void Graphics::DrawLinie(const Vector2 p1, const Vector2 p2, const Color color)
{
  int setColor;
  setColor = this->SetColor(color);
  SDL_RenderDrawLine(this->_renderer, p1.x, p1.y, p2.x, p2.y);
}

void Graphics::flip() 
{
  SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() 
{
  SDL_SetRenderDrawColor(this->_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const 
{
  return this->_renderer;
}