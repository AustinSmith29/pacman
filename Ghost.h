#ifndef _GHOST_H
#define _GHOST_H

#include "Graphics.h"
#include "Maze.h"

enum GhostState {CHASE, SCATTER, SCARED};

class Ghost
{
 public:
  void init(GraphicsLoader *loader);

  /** Performs the logic for the ghost depending on its state. **/
  void update(Maze &maze);
  
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
  int velocity_x, velocity_y;
  int target_x, target_y;     /// Destination of pathfinding
  std::stack<std::pair<int,int>> path;
  bool has_path;
  GhostState current_state;
  Sprite sprite;
};
#endif
