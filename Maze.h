/** @file Maze.h
 *  @brief Contains the definition for the maze that pacman/ghosts navigate.
 *
 *  @author Austin Smith
 *  @bugs No known bugs
 **/

#ifndef _MAZE_H
#define _MAZE_H

#include "Graphics.h"

#include <map>
#include <stack>

#define MAZE_WIDTH   28
#define MAZE_HEIGHT  31
#define TILE_SIZE    16

typedef std::pair<int,int> cell; /// represents grid cell in Maze

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

  friend std::stack<cell> dikstra(int target_x, int target_y,
				  int from_x, int from_y, Maze &maze);
  
  friend std::stack<cell> get_path(std::map<cell, cell> parent,
				   cell target,
				   Maze &maze);

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

  /** Used to snap pacman/ghost to the center of the current tile they are in along given coordinate.
   *  Primarily used when changing directions at intersections to keep everything aligned.
   *  @param x the screen position that will become aligned.
   *  @return the snapped screen position
   **/
  int snap_x(int x);
  int snap_y(int y);

  void draw(SDL_Renderer *renderer);

 private:
  Sprite tile_sprites[10];
  std::map<cell, Tile> tiles;
  std::map<cell, std::vector<cell>> neighbors; /// cell --> vector of surrounding cells
  int x_ofnfset, y_offset;  /// screen_location of top-left corner of maze

  /**
   * Checks if screen coordinates are within designated area for maze.
   * @param x the x SCREEN coordinate
   * @param y the y SCREEN coordinate
   * @return true if in bounds, false otherwise.
   **/
  bool in_bounds(int x, int y);

  /**
   * Converts screen coordinates into grid coordinates that can index an array.
   **/
  void screen_to_grid(int &screen_x, int &screen_y);

  /**
   * Converts grid coordinates into screen coordinates.
   **/
  void grid_to_screen(int &grid_x, int &grid_y);

  /**
   * Called in constructor. Forms connections between cells to create graph.
   **/
  void setup_neighbors();

  /** 
   * Given a cell in the map, return vector of cells around that cell that are passable.
   **/
  std::vector<cell> get_neighbors(cell at);
};

std::stack<cell> dikstra(int target_x, int target_y,
			 int from_x, int from_y,
			 Maze &maze);

std::stack<cell> get_path(std::map<cell, cell> parent,
			  cell target,
			  Maze &maze);
#endif
