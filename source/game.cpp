#include <SDL.h>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include <algorithm>

/* Game class
*  This class holds all information for our main game loop
*/


namespace
{
  const int FPS = 50;
  const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  this->gameLoop();

}

Game::~Game()
{

}

void Game::gameLoop()
{
  Graphics graphics;
  SDL_Event event;
  Input input;

  this->_level = Level("Map1", Vector2(100, 100), graphics);
  this->_player = Player(graphics, this->_level.GetPlayerSpawnPoint());

  int LAST_UPDATE_TIME = SDL_GetTicks();
  //Start the game loop
  while (true)
  {
    input.beginNewFrame();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        return;
      }
      else if (event.type == SDL_KEYDOWN)
      {
        if (event.key.repeat == 0)
        {
          input.keyDownEvent(event);
        }
      }
      else if (event.type = SDL_KEYUP)
      {
        input.keyUpEvent(event);
      }
    }
    if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
    {
      return;
    }
    if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true)
    {
      this->_player.moveLeft();
    }
    else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true)
    {
      this->_player.moveRight();
    }

    if (input.wasKeyPressed(SDL_SCANCODE_UP) == true)
    {
      this->_player.jump();
    }


    if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT))
    {
      this->_player.stopMovingX();
    }


    const int CURRENT_TIME_MS = SDL_GetTicks();
    int ELAPSET_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
    this->update(std::min(ELAPSET_TIME_MS, MAX_FRAME_TIME));
    LAST_UPDATE_TIME = CURRENT_TIME_MS;

    this->draw(graphics);
  }
}

void Game::draw(Graphics &graphics)
{
  graphics.clear();

  this->_level.draw(graphics);
  this->_player.draw(graphics);

  graphics.flip();
}

void Game::update(float elapsedTime)
{
  this->_player.update(elapsedTime);
  this->_level.update(elapsedTime);

  //Check collisions
  std::vector<Rectangle> others = this->_level.CheckTileCollision(this->_player.GetBoundingBox());
  if (others.size() > 0)
  {
    //Player collided with at least 1 tile. Handle it.
    this->_player.HandleTileCollisions(others);
  }

  //Check slopes
  std::vector<Slope> otherSlopes = this->_level.CheckSlopeCollision(this->_player.GetBoundingBox());
  if (otherSlopes.size() > 0)
  {
    this->_player.HandleSlopeCollision(otherSlopes);
  }
}