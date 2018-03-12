#include "GameState.h"
#include <iostream>

#define UP     0
#define DOWN   1
#define RIGHT  2
#define LEFT   3

GameState::GameState(GraphicsLoader *graphics_loader)
{
  input.bind(SDLK_UP, "up");
  input.bind(SDLK_RIGHT, "right");
  input.bind(SDLK_DOWN, "down");
  input.bind(SDLK_LEFT, "left");
  input.bind(SDLK_SPACE, "stop");

  pacman_right = graphics_loader->load_animation("pacman_right.dat");
  pacman_up = graphics_loader->load_animation("pacman_up.dat");
  pacman_left = graphics_loader->load_animation("pacman_left.dat");
  pacman_down = graphics_loader->load_animation("pacman_down.dat");
  pacman = &pacman_left;
  maze = graphics_loader->load_sprite("maze.bmp");
  vx = -2;
  vy = 0;
  x = 318;
  y = 272;
  direction = LEFT;
  next_direction = LEFT;
  changed = false;
}

void
GameState::handle_input(SDL_Event &e)
{
  std::string action = input.get_action(e);
  if (action == "up") {
    next_direction = UP;
  }
  else if (action == "right") {
    next_direction = RIGHT;
  }
  else if (action == "left") {
    next_direction = LEFT;
  }
  else if (action == "down") {
    next_direction = DOWN;
  }
  else if (action == "stop") {
    vx = 0;
    vy = 0;
  }
  changed = (next_direction == direction) ? false : true;
}

void
GameState::update_logic()
{
  x += vx;
  y += vy;

  if (next_direction == UP && changed && maze_obj.item_at(x+8, y-4) != TileType::WALL)
    {
      vx = 0;
      vy = -2;
      x = maze_obj.snap_x(x+8);
      pacman = &pacman_up;
      direction = next_direction;
    }
  if (next_direction == RIGHT && changed && maze_obj.item_at(x+24, y+8) != TileType::WALL)
    {
      vx = 2;
      vy = 0;
      y = maze_obj.snap_y(y+8);
      pacman = &pacman_right;
      direction = next_direction;
    }
  if (next_direction == DOWN && changed && maze_obj.item_at(x+8, y+28) != TileType::WALL)
    {
      vx = 0;
      vy = 2;
      x = maze_obj.snap_x(x+8);
      pacman = &pacman_down;
      direction = next_direction;
    }
  if (next_direction == LEFT && changed && maze_obj.item_at(x-4, y+8) != TileType::WALL)
    {
      vx = -2;
      vy = 0;
      y = maze_obj.snap_y(y+8);
      pacman = &pacman_left;
      direction = next_direction;
    }
  // Check for collisions
  if (vx > 0)
    {
      if (maze_obj.item_at(x+16, y+8) == TileType::WALL)
	{
	  vx = 0;
	  x = maze_obj.snap_x(x);
	}
    }
  if (vx < 0)
    {
      if (maze_obj.item_at(x, y+8) == TileType::WALL)
	{
	  vx = 0;
	  x = maze_obj.snap_x(x+8);
	}
    }
  if (vy > 0)
    {
      if (maze_obj.item_at(x+8, y+16) == TileType::WALL)
	{
	  vy = 0;
	  y = maze_obj.snap_y(y);
	}
    }
  if (vy < 0)
    {
      if (maze_obj.item_at(x+8, y) == TileType::WALL)
	{
	  vy = 0;
	  y = maze_obj.snap_y(y+8);
	}
    }

}

void 
GameState::draw(SDL_Renderer *renderer)
{
  maze_obj.draw(renderer);
  play_animation(renderer, *pacman, x, y);
}
