#ifndef _GAME_H
#define _GAME_H

#include "Graphics.h"

#include <SDL2/SDL.h>
#include <stack>

class State;
class Game
{
 public:
  Game();
  ~Game();

  void push_state(State *state);
  void loop();

 private:
  State *current_state;
  std::stack<State*> state_stack;
  SDL_Window *window;
  SDL_Renderer *renderer;
  GraphicsLoader graphics_registry;
  unsigned int fps;
  bool quit;
};

#endif
