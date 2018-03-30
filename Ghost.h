#ifndef _GHOST_H
#define _GHOST_H

#include "Graphics.h"
#include "Maze.h"

#include <functional>

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
  static void red_ghost(Ghost &ghost, Maze &maze, AIState &state);
  static void blue_ghost(Ghost &ghost, Maze &maze, AIState &state);
  static void pink_ghost(Ghost &ghost, Maze &maze, AIState &state);
  static void orange_ghost(Ghost &ghost, Maze &maze, AIState &state);

 private:
  ChaseFunction() { }
};

enum GhostState {CHASE, SCATTER, SCARED};

class Ghost
{
 public:
  void init(GraphicsLoader *loader, int x, int y,
            std::function<void(Ghost&, Maze&, AIState&)> chase,
            int scat_x, int scat_y);

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
  Sprite sprite;
  GhostState current_state;
  int path_timer;             /// When path_ticks > path_timer recalculate path.
  int path_ticks;
  bool has_path;
  std::stack<std::pair<int,int>> path;
  /// Each ghost chases pacman using a unique method.
  std::function<void(Ghost&, Maze&, AIState&)> chase; 

  friend class ChaseFunction;
};
#endif
