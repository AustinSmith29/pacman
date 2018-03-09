#include "GameState.h"

GameState::GameState(GraphicsLoader *graphics_loader)
{
  input.bind(SDLK_UP, "up");
  input.bind(SDLK_RIGHT, "right");
  input.bind(SDLK_DOWN, "down");
  input.bind(SDLK_LEFT, "left");

  pacman_right = graphics_loader->load_animation("pacman_right.dat");
  pacman_up = graphics_loader->load_animation("pacman_up.dat");
  pacman_left = graphics_loader->load_animation("pacman_left.dat");
  pacman_down = graphics_loader->load_animation("pacman_down.dat");
  pacman = &pacman_right;
  maze = graphics_loader->load_sprite("maze.bmp");
  vx = vy = 0;
  x = 340;
  y = 260;
}

void
GameState::handle_input(SDL_Event &e)
{
  std::string action = input.get_action(e);
  if (action == "up") {
    vy = -3;
    vx = 0;
    pacman = &pacman_up;
  }
  else if (action == "right") {
    vy = 0;
    vx = 3;
    pacman = &pacman_right;
  }
  else if (action == "left") {
    vx = -3;
    vy = 0;
    pacman = &pacman_left;
  }
  else if (action == "down") {
    vx = 0;
    vy = 3;
    pacman = &pacman_down;
  }
}

void
GameState::update_logic()
{
  x += vx;
  y += vy;
}

void 
GameState::draw(SDL_Renderer *renderer)
{
  draw_sprite(renderer, maze, 0, 0);
  play_animation(renderer, *pacman, x, y);
}
