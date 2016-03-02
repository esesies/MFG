#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "globals.h"
#include "tile.h"


class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level
{
public:
  Level();
  Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics);
  ~Level();

  void update(float elapsedTime);
  void draw(Graphics &graphics);

private:
  std::string _mapName;
  Vector2 _spawnPoint;

  Vector2 _size; //Width and height of the entire map
  Vector2 _tileSize;
  
  SDL_Texture* _backgroundTexture;

  std::vector<Tile> _tileList;
  std::vector<Tileset> _tilesets;
  

  /* void loadMap
   * Loads a map
   * Es privada porque sólo la llamamos desde esta clase misma
   */
  void loadMap(std::string mapName, Graphics &graphics);
};


//Tileset structure
struct Tileset
{
  SDL_Texture* Texture; //Tileset image
  int FirstGid;

  Tileset()
  {
    this->FirstGid = -1;
  }

  Tileset(SDL_Texture* texture, int firstGid)
  {
    this->Texture = texture;
    this->FirstGid = firstGid;
  }
};


#endif //LEVEL_H