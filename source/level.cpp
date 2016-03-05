#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "tinyxml2.h"


#include <SDL.h>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;


Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
  _mapName(mapName),
  _spawnPoint(spawnPoint),
  _size(Vector2(0,0))
{
  this->loadMap(mapName, graphics);
}

Level::~Level() {}


void Level::loadMap(std::string mapName, Graphics &graphics)
{
  //Parse the .tmx file

  XMLDocument doc;
  std::stringstream ss;
  ss << "../content/maps/" << mapName << ".tmx";
  doc.LoadFile(ss.str().c_str()); //Tenemos que convertir a c_str (char*)

  XMLElement* mapNode = doc.FirstChildElement("map"); //The first child called "map" (Ver en el tmx)

  //Get the width and the hight of the whole map and store it in _size.
  int width, height;
  mapNode->QueryIntAttribute("width", &width);
  mapNode->QueryIntAttribute("height", &height);

  this->_size = Vector2(width, height);

  //Get the width and the height of the tiles and store it in _tileSize.
  int tileWidth, tileHeight;
  mapNode->QueryIntAttribute("tilewidth", &tileWidth);
  mapNode->QueryIntAttribute("tileheight", &tileHeight);

  this->_tileSize = Vector2(tileWidth, tileHeight);

  //Loading the tilesets
  XMLElement* pTileset = mapNode->FirstChildElement("tileset");
  if (pTileset != NULL)
  {
    while (pTileset)
    {
      int firstgid;
      const char* source = pTileset->FirstChildElement("image")->Attribute("source");
      char* path;
      std::stringstream ss;
      ss << source;
      pTileset->QueryIntAttribute("firstgid", &firstgid);
      SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(),
                                                      graphics.loadImage(ss.str()));
      this->_tilesets.push_back(Tileset(tex, firstgid));

      pTileset = pTileset->NextSiblingElement("tileset");
    }
  }

  //Loading the layers
  XMLElement* pLayer = mapNode->FirstChildElement("layer");
  if (pLayer != NULL)
  {
    while (pLayer)
    {
      //Loading the data element
      XMLElement* pData = pLayer->FirstChildElement("data");
      if (pData != NULL)
      {
        while (pData)
        {
          //Loading the tile element
          XMLElement* pTile = pData->FirstChildElement("tile");
          if (pTile != NULL)
          {
            int tileCounter = 0;
            while (pTile)
            {
              //Build each individual tile here
              //If gid is 0, no tile should be drawn. Continue loop.
              if (pTile->IntAttribute("gid") == 0)
              {
                tileCounter++;
                if (pTile->NextSiblingElement("tile"))
                {
                  pTile = pTile->NextSiblingElement("tile");
                  continue;
                }
                else
                {
                  break;
                }
              }

              //Get the tileset for this specific git
              int gid = pTile->IntAttribute("gid");
              Tileset tls;
              for (int i = 0; i < this->_tilesets.size(); i++)
              {
                if (this->_tilesets[i].FirstGid <= gid)
                {
                  //This is the tileset we want
                  tls = this->_tilesets.at(i);
                  break;
                }
              }
 
              if (tls.FirstGid == -1)
              {
                //No tileset was found for this gid
                tileCounter++;
                if (pTile->NextSiblingElement("tile"))
                {
                  pTile = pTile->NextSiblingElement("tile");
                  continue;
                }
                else
                {
                  break;
                }
              }

              //Get the position of the tile in the level
              int xx, yy;              
              xx = (tileCounter % width) * tileWidth;
              yy = (tileCounter / width) * tileHeight;
              
              Vector2 finalTilePosition(xx, yy);

              //Get the position of the tile in the tileset
              int tilesetWidth, tilesetHeight;
              SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
              int tsxx = ((gid % (tilesetWidth / tileWidth)) - 1) * tileWidth;
              int tsyy = (gid / (tilesetWidth / tileWidth)) * tileHeight;

              Vector2 finalTilesetPosition(tsxx, tsyy);

              //Build the actual tile and add it to the level's tile list
              Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
              this->_tileList.push_back(tile);
              tileCounter++;

              pTile = pTile->NextSiblingElement("tile");
            }              
          }
          pData = pData->NextSiblingElement("data");
        }
      }
      pLayer = pLayer->NextSiblingElement("layer");
    }
  }

  //Parse out the collisions
  XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
  if (pObjectGroup != NULL)
  {
    while (pObjectGroup)
    {
      const char* name = pObjectGroup->Attribute("name");
      std::stringstream ss;
      ss << name;
      if (ss.str() == "collisions")
      {
        XMLElement* pObject = pObjectGroup->FirstChildElement("object");
        if (pObject != NULL)
        {
          while (pObject)
          {
            float x, y, width, height;
            x = pObject->FloatAttribute("x");
            y = pObject->FloatAttribute("y");
            width = pObject->FloatAttribute("width");
            height = pObject->FloatAttribute("height");
            this->_collisionRects.push_back(
              Rectangle(
                std::ceil(x) * globals::SPRITE_SCALE, 
                std::ceil(y) * globals::SPRITE_SCALE,
                std::ceil(width) * globals::SPRITE_SCALE,
                std::ceil(height) * globals::SPRITE_SCALE));

            pObject = pObject->NextSiblingElement("object");
          }
        }

      }
      else if (ss.str() == "spawn points")
      {
        XMLElement* pObject = pObjectGroup->FirstChildElement("object");
        if (pObject != NULL)
        {
          while (pObject)
          {
            float x = pObject->FloatAttribute("x");
            float y = pObject->FloatAttribute("y");
            const char* name = pObject->Attribute("name");
            std::stringstream ss;
            ss << name;
            if (ss.str() == "player")
            {
              this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
                std::ceil(y) * globals::SPRITE_SCALE);
            }

            pObject = pObject->NextSiblingElement("object");
          }
        }
      }
      

      pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
    }
  }







  //Temporary code to load the background
  //this->_backgroundTexture = SDL_CreateTextureFromSurface(
  //                           graphics.getRenderer(),
  //                           graphics.loadImage("../content/background/bkBlue.png"));
  //this->_size = Vector2(1280, 960);
}

void Level::update(float elapsedTime)
{

}

void Level::draw(Graphics &graphics)
{
  //Draw the background
  for (int i = 0; i < this->_tileList.size(); i++)
  {
    this->_tileList.at(i).draw(graphics);
  }

  //SDL_Rect sourceRect = {0, 0, 64, 64};
  //SDL_Rect destRect;

  //for (int x = 0; x < this->_size.x / 64; x++)
  //{
  //  for (int y = 0; y < this->_size.y / 64; y++)
  //  {
  //    destRect.x = x * 64 * globals::SPRITE_SCALE;
  //    destRect.y = y * 64 * globals::SPRITE_SCALE;
  //    destRect.w = 64 * globals::SPRITE_SCALE;
  //    destRect.h = 64 * globals::SPRITE_SCALE;
  //    graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
  //  }
  //}
}

std::vector<Rectangle> Level::CheckTileCollision(const Rectangle& other)
{
  std::vector<Rectangle> others;
  
  for (int i = 0; i < this->_collisionRects.size(); i++)
  {
    if (this->_collisionRects.at(i).CollidesWith(other))
    {
      others.push_back(this->_collisionRects.at(i));
    }
  }
  return others;
}

const Vector2 Level::GetPlayerSpawnPoint(void) const { return this->_spawnPoint; }