#include "Game.h"
#include "GameState.h"

#include <stdexcept>
#include <iostream>

Game::Game()
{
  current_state = nullptr;
  fps = 30;
  quit = false;
  SDL_Init(SDL_INIT_EVERYTHING);

  if(SDL_CreateWindowAndRenderer(640, 500, SDL_WINDOW_RESIZABLE, 
				 &window, &renderer))
    {
      std::cout << "Could not create window and renderer: " << SDL_GetError() << std::endl;
      throw std::runtime_error("Could not start up.");
    }

  // Register Services
  graphics_registry.register_service(renderer);

  // Set up Game States
  State *game_state = new GameState(&graphics_registry);
  push_state(game_state);
}

Game::~Game()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
Game::push_state(State *state)
{
  state_stack.push(state);
  current_state = state;
}

void
Game::loop()
{
  while (!state_stack.empty() && !quit)
    {
      current_state = state_stack.top();
      state_stack.pop();
      while (current_state->is_running())
	{
	  unsigned int start = SDL_GetTicks();
	  // Handle Quit specific events.
	  SDL_Event e;
	  while (SDL_PollEvent(&e) != 0)
	    {
	      if (e.type == SDL_QUIT)
		{
		  quit = true;
		  break;
		}
	      current_state->handle_input(e);
	    }
	  if (quit)
	    break;
	  current_state->update_logic();

	  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	  SDL_RenderClear(renderer);
	  current_state->draw(renderer);
	  SDL_RenderPresent(renderer);
	  unsigned int end = SDL_GetTicks();
	  if ((end - start) < 1000 / fps)
	    {
	      SDL_Delay( (1000 / fps) - (end - start) );
	    }
	}
    }
}
