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
  score = 0;
  num_pellets = 244;
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
      int ghost_x = ghosts[i]->get_x();
      int ghost_y = ghosts[i]->get_y();
      maze_obj.screen_to_grid(ghost_x, ghost_y);
      int pac_x = px;
      int pac_y = py;
      maze_obj.screen_to_grid(pac_x, pac_y);
      if (pac_x == ghost_x && pac_y == ghost_y)
        {
          if (ghosts[i]->is_eatable())
            {
              ghosts[i]->set_state(GhostState::EATEN);
            }
          else
            {
              pacman.die();
            }
        }
    }

  if (maze_obj.item_at(px + 8, py + 8) == TileType::DOT)
    {
      score += 10;
      maze_obj.remove_at(px, py);
      num_pellets--;
    }

  if (maze_obj.item_at(px + 8, py + 8) == TileType::BIGDOT)
    {
      score += 50;
      maze_obj.remove_at(px, py);
      // replace with for_each
      for (int i = 0; i < ghosts.size(); i++)
        {
          ghosts[i]->set_state(GhostState::SCARED);
        }
    }

  if (maze_obj.pass_tunnel_right(px, py))
    {
      pacman.tunnel_right();
    }
  else if (maze_obj.pass_tunnel_left(px, py))
    {
      pacman.tunnel_left();
    }
}

void
GameState::draw(SDL_Renderer *renderer)
{
  //pacman.draw(renderer); // pacman drawn first cuz of side tunnel hack.
  maze_obj.draw(renderer);
  pacman.draw(renderer); // pacman drawn first cuz of side tunnel hack.
  for (int i = 0; i < ghosts.size(); i++)
    {
      ghosts[i]->draw(renderer);
    }
}
