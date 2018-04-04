#ifndef _GHOST_H
#define _GHOST_H

#include "Graphics.h"
#include "Maze.h"
#include "Timer.h"

#include <functional>
#include <memory>

#define SCARED_SPEED 1

/**
 * AIState contains information that is important for Ghost path calculations.
 **/
struct AIState
{
  int pacman_x;
  int pacman_y;
  int pacman_direction;
  bool fruit_exists;
};

/**
 * ChaseFunction is a collection of functions ghosts can use to chase pacman.
 **/
class Ghost;
class ChaseFunction
{
 public:
  /** Directly chase pacman **/
  static void red_ghost(Ghost &ghost, Maze &maze, AIState &state);

  /** Go to place 4 cells behind pacman. Not used in original game. **/
  static void blue_ghost(Ghost &ghost, Maze &maze, AIState &state);

  /** Go to place 4 cells in front of pacman. **/
  static void pink_ghost(Ghost &ghost, Maze &maze, AIState &state);

  /** If > 8 cells away from pacman, directly chase pacman, else go
   *  to scatter cell.
   **/
  static void orange_ghost(Ghost &ghost, Maze &maze, AIState &state);

 private:
  ChaseFunction() { }
};

enum GhostState { CHASE, SCATTER, SCARED };
enum GhostType { RED, BLUE, ORANGE, PINK };

class Ghost
{
 public:
  friend Ghost ghost_factory(GhostType type, GraphicsLoader *loader);

  Ghost(int scatter_x, int scatter_y,
        std::function<void(Ghost&, Maze&, AIState&)> chase);

  void init(GraphicsLoader *loader, std::string sprite_filepath, int x, int y);

  /** Performs the logic for the ghost depending on its state. **/
  void update(Maze &maze, AIState &state); 

  /** Set ghost target to specified coordinates. Ghost will
   *  begin moving towards the given coordinates.
   *
   * @param x the target x screen coordinate
   * @param y the target y screen coordinate
   **/
  void go_to(int x, int y, Maze &maze);

  void set_state(GhostState new_state);

  void draw(SDL_Renderer *renderer);

 private:
  int x, y;
  int scatter_x, scatter_y;
  int speed;
  int current_speed;          /// speed changes when ghost becomes scared.
  Sprite sprite;
  Sprite scared_sprite;
  Sprite *current_sprite;
  GhostState current_state;
  bool has_path;
  std::stack<std::pair<int,int>> path;
  /// Each ghost chases pacman using a unique method.
  std::function<void(Ghost&, Maze&, AIState&)> chase; 
  Timer scatter_timer;
  Timer scared_timer; 
  Timer path_timer;
  Timer ghost_house_timer;

  friend class ChaseFunction;
};

std::unique_ptr<Ghost> ghost_factory(GraphicsLoader *loader, GhostType type);
#endif
