#ifndef _PACMAN_H
#define _PACMAN_H

#include "Graphics.h"
#include "Maze.h"

enum Direction { LEFT, UP, RIGHT, DOWN };

class Pacman
{
 public:
  void init(GraphicsLoader *loader);
  
  void move_left();
  void move_up();
  void move_right();
  void move_down();

  void update(Maze &maze);
  void draw(SDL_Renderer *renderer);
  
 private:
  int x, y;
  int velocity_x, velocity_y;
  Direction direction;
  Direction next_direction;
  bool changed_direction;

  Animation pacman_left;
  Animation pacman_up;
  Animation pacman_right;
  Animation pacman_down;
  Animation *current_animation;

  void change_direction(Maze &maze);
  void handle_collisions(Maze &maze);
};
#endif
