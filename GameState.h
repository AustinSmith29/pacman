#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "State.h"
#include "Input.h"
#include "Graphics.h"
#include "Maze.h"
#include "Pacman.h"
#include "Ghost.h"

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
  Pacman pacman;
  Sprite maze;
  Maze maze_obj;
  Ghost test_bitch;
  Ghost test_bitch2;
  Ghost test_bitch3;
  Ghost test_bitch4;
};
#endif
