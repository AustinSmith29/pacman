#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "State.h"
#include "Input.h"
#include "Graphics.h"
#include "Maze.h"

class GameState : public State
{
 public:
  GameState(GraphicsLoader *graphics_loader);
  ~GameState() {};

  void handle_input(SDL_Event &e) override; 
  void update_logic() override; 
  void draw(SDL_Renderer *renderer) override;
  bool is_running() override { return true; }

 private:
  Input input;
  Animation pacman_right;
  Animation pacman_up;
  Animation pacman_left;
  Animation pacman_down;
  Animation *pacman;
  Sprite maze;
  Maze maze_obj;
 
  int vx, vy;
  int x, y;
  int direction;
  int next_direction;
  bool changed;
};
#endif
