#include "GameState.h"
#include <iostream>

GameState::GameState(GraphicsLoader *graphics_loader) 
{
  input.bind(SDLK_UP, "up");
  input.bind(SDLK_RIGHT, "right");
  input.bind(SDLK_DOWN, "down");
  input.bind(SDLK_LEFT, "left");
  input.bind(SDLK_SPACE, "stop");

  pacman.init(graphics_loader);
  ghosts.push_back(ghost_factory(graphics_loader, GhostType::RED));
  ghosts.push_back(ghost_factory(graphics_loader, GhostType::BLUE));
  ghosts.push_back(ghost_factory(graphics_loader, GhostType::ORANGE));
  ghosts.push_back(ghost_factory(graphics_loader, GhostType::PINK));
  maze_obj.init(graphics_loader);
  maze = graphics_loader->load_sprite("maze.bmp");
}

void
GameState::handle_input(SDL_Event &e)
{
  std::string action = input.get_action(e);
  if (action == "up") {
    pacman.move_up();
  }
  else if (action == "right") {
    pacman.move_right();
  }
  else if (action == "left") {
    pacman.move_left();
  }
  else if (action == "down") {
    pacman.move_down();
  }
}

void
GameState::update_logic()
{
  pacman.update(maze_obj);
  int px = pacman.get_x();
  int py = pacman.get_y();
  AIState state;
  state.pacman_x = px;
  state.pacman_y = py;
  for (int i = 0; i < ghosts.size(); i++)
    {
      ghosts[i]->update(maze_obj, state);
    }

  if (maze_obj.item_at(px, py) == TileType::DOT)
    {
      maze_obj.remove_at(px, py);
    }

  if (maze_obj.item_at(px, py) == TileType::BIGDOT)
    {
      maze_obj.remove_at(px, py);
      for (int i = 0; i < ghosts.size(); i++)
        {
          ghosts[i]->set_state(GhostState::SCARED);
        }
    }
}

void 
GameState::draw(SDL_Renderer *renderer)
{
  maze_obj.draw(renderer);
  for (int i = 0; i < ghosts.size(); i++)
    {
      ghosts[i]->draw(renderer);
    }
  pacman.draw(renderer);
}
