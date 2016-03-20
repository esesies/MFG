#include "level.h"
#include "graphics.h"
#include "tinyxml2.h"
#include "utils.h"


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

  //Loading the layers
  XMLElement* pLayer = mapNode->FirstChildElement("layer");
  while (pLayer)
  {
    //Loading the data element
    XMLElement* pData = pLayer->FirstChildElement("data");
    while (pData)
    {
      //Loading the tile element
      XMLElement* pTile = pData->FirstChildElement("tile");
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
        int closest = 0;
        for (int i = 0; i < this->_tilesets.size(); i++)
        {
          if (this->_tilesets[i].FirstGid <= gid)
          {
            if (this->_tilesets[i].FirstGid > closest)
            {
              closest = this->_tilesets[i].FirstGid;
              //This is the tileset we want
              tls = this->_tilesets.at(i);
            }          
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
        int tsyy = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth)) * tileHeight;

        Vector2 finalTilesetPosition(tsxx, tsyy);

        //Build the actual tile and add it to the level's tile list
        Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
        this->_tileList.push_back(tile);
        tileCounter++;

        pTile = pTile->NextSiblingElement("tile");
      }              
      pData = pData->NextSiblingElement("data");
    }
    pLayer = pLayer->NextSiblingElement("layer");
  }
  
  //Parse out the collisions
  XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
  while (pObjectGroup)
  {
    const char* name = pObjectGroup->Attribute("name");
    std::stringstream ss;
    ss << name;
    if (ss.str() == "collisions")
    {
      XMLElement* pObject = pObjectGroup->FirstChildElement("object");
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
    else if (ss.str() == "slopes")
    {
      XMLElement* pObject = pObjectGroup->FirstChildElement("object");
      while (pObject)
      {
        std::vector<Vector2> points;
        Vector2 p1;           
        p1.x = std::ceil(pObject->FloatAttribute("x"));
        p1.y = std::ceil(pObject->FloatAttribute("y"));

        XMLElement* pPolyline = pObject->FirstChildElement("polyline");
        if (pPolyline != NULL)
        {
          std::vector<std::string> pairs;
          const char* pointString = pPolyline->Attribute("points");
          std::stringstream ss;
          ss << pointString;
          Utils::Split(ss.str(), pairs, ' ');
          //Now we have each of the pairs.
          for (int i = 0; i < pairs.size(); i++)
          {
            std::vector<std::string> ps;
            Utils::Split(pairs.at(i), ps, ',');
            points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
          }
        }

        for (int i = 0; i < points.size(); i+=2)
        {
          this->_slopes.push_back(Slope(
            Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
                    ((p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE)),
            Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
                    (p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE))
            );
        }
        pObject = pObject->NextSiblingElement("object");
      }
    }
    else if (ss.str() == "spawn points")
    {
      XMLElement* pObject = pObjectGroup->FirstChildElement("object");
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
    pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
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
}

void Level::drawCollisions(Graphics &graphics)
{
  for (unsigned int i = 0; i < this->_collisionRects.size(); i++)
  {
    SDL_Rect fillRect = { this->_collisionRects.at(i).GetLeft(), this->_collisionRects.at(i).GetTop(),
      this->_collisionRects.at(i).GetWidth(), this->_collisionRects.at(i).GetHeight() };
    graphics.DrawRectangle(&fillRect, BLUE);
  }
  for (unsigned int i = 0; i < this->_slopes.size(); i++)
  {
    graphics.DrawLinie(this->_slopes.at(i).GetP1(), this->_slopes.at(i).GetP2(), WHITE);
  }
}

std::vector<Rectangle> Level::CheckTileCollision(const Rectangle& other)
{
  std::vector<Rectangle> others;
  
  for (int i = 0; i < this->_collisionRects.size(); i++)
    if (this->_collisionRects.at(i).CollidesWith(other))
      others.push_back(this->_collisionRects.at(i));

  return others;
}

std::vector<Slope> Level::CheckSlopeCollision(const Rectangle & other)
{
  std::vector<Slope> others;

  for (int i = 0; i < this->_slopes.size(); i++)
    if (this->_slopes.at(i).CollidesWith(other))
      others.push_back(this->_slopes.at(i));

  return others;
}

const Vector2 Level::GetPlayerSpawnPoint(void) const { return this->_spawnPoint; }