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
  test_bitch.init(graphics_loader, 318, 20, ChaseFunction::red_ghost, 318, 20);
  test_bitch2.init(graphics_loader, 318, 272, ChaseFunction::blue_ghost, 318, 272);
  test_bitch3.init(graphics_loader, 420, 272, ChaseFunction::pink_ghost, 420, 272);
  test_bitch4.init(graphics_loader, 250, 272, ChaseFunction::orange_ghost, 250, 272);
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
  test_bitch.update(maze_obj, state);
  test_bitch2.update(maze_obj, state);
  test_bitch3.update(maze_obj, state);
  test_bitch4.update(maze_obj, state);

  if (maze_obj.item_at(px, py) == TileType::DOT)
    {
      maze_obj.remove_at(px, py);
    }
}

void 
GameState::draw(SDL_Renderer *renderer)
{
  maze_obj.draw(renderer);
  test_bitch.draw(renderer);
  test_bitch2.draw(renderer);
  test_bitch3.draw(renderer);
  test_bitch4.draw(renderer);
  pacman.draw(renderer);
}
