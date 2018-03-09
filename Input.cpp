#include "Input.h"

void
Input::bind(SDL_Keycode code, std::string action)
{
  input_map.insert( std::pair<SDL_Keycode, std::string>(code, action) );
}

std::string
Input::get_action(SDL_Event &e)
{
  auto it = input_map.find(e.key.keysym.sym);
  if (it != input_map.end())
    return input_map[e.key.keysym.sym];
  return std::string("NONE");
}
