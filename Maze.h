/** @file Maze.h
 *  @brief Contains the definition for the maze that pacman/ghosts navigate.
 *
 *  @author Austin Smith
 *  @bugs No know bugs
 **/

#ifndef _MAZE_H
#define _MAZE_H

#include "Graphics.h"

#include <map>

#define MAZE_WIDTH   28
#define MAZE_HEIGHT  31
#define TILE_SIZE    16

enum TileType
  {
    OPEN,
    WALL,
    TUNNEL,
    DOT,
    BIGDOT,
    JUNCTOIN,
    GATE,
    CHERRY,
    STRAWBERRY,
    ORANGE,
    APPLE,
    MELON,
    UNKNOWN
  };

struct Tile
{
  Tile() = default;
Tile(int x, int y, TileType type) : x(x), y(y), type(type) {}
  int x, y;
  TileType type;
};

class Maze
{
 public:
  Maze();

  void reset();
  
  /**
   * Returns the tile at the specified screen coordinates.
   * @param x the x SCREEN coordinate
   * @param y the y SCREEN coordinate
   * @return the tile type
   **/
  TileType item_at(int x, int y);

  /**
   * Places the specified Tile type at the given coordinates.
   **/
  void insert_at(TileType obj, int x, int y);

  /**
   * Sets the Tile at the given coordinates to an OPEN tile type.
   **/
  void remove_at(int x, int y);

  void draw(SDL_Renderer *renderer);

 private:
  Sprite tile_sprites[10];
  std::map<std::pair<int, int>, Tile> tiles;
  int x_offset, y_offset;  /// screen_location of top-left corner of maze
  bool in_bounds(int x, int y);
};
#endif
