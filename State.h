#ifndef _STATE_H
#define _STATE_H

#include <SDL2/SDL.h>

class State
{
 public:
  virtual void handle_input(SDL_Event &e) = 0;
  virtual void update_logic() = 0;
  virtual void draw(SDL_Renderer *renderer) = 0;
  virtual bool is_running() = 0;
};
#endif
