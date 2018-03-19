#include "Pacman.h"

void
Pacman::init(GraphicsLoader *loader)
{
  pacman_right = loader->load_animation("pacman_right.dat");
  pacman_up    = loader->load_animation("pacman_up.dat");
  pacman_left  = loader->load_animation("pacman_left.dat");
  pacman_down  = loader->load_animation("pacman_down.dat");
  current_animation = &pacman_left;

  x = 318;
  y = 272;
  velocity_x = -2;
  velocity_y = 0;
  direction = LEFT;
  next_direction = LEFT;
  changed_direction = false;
}

void
Pacman::move_left()
{
  next_direction = LEFT;
  changed_direction = (next_direction == direction) ? false : true;
}

void
Pacman::move_up()
{
  next_direction = UP;
  changed_direction = (next_direction == direction) ? false : true;
}

void
Pacman::move_right()
{
  next_direction = RIGHT;
  changed_direction = (next_direction == direction) ? false : true;
}

void
Pacman::move_down()
{
  next_direction = DOWN;
  changed_direction = (next_direction == direction) ? false : true;
}

void
Pacman::update(Maze &maze)
{
  x += velocity_x;
  y += velocity_y;

 if (changed_direction)
    change_direction(maze);

  handle_collisions(maze);
}

void
Pacman::draw(SDL_Renderer *renderer)
{
  play_animation(renderer, *current_animation, x, y);
}

void
Pacman::change_direction(Maze &maze)
{
  switch (next_direction)
    {
    case UP:
      if (maze.item_at(x+8, y-4) != TileType::WALL)
	{
	  velocity_x = 0;
	  velocity_y = -2;
	  x = maze.snap_x(x+8);
	  current_animation = &pacman_up;
	  direction = UP;
	}
      break;
	  
    case RIGHT:
      if (maze.item_at(x+24, y+8) != TileType::WALL)
	{
	  velocity_x = 2;
	  velocity_y = 0;
	  y = maze.snap_y(y+8);
	  current_animation = &pacman_right;
	  direction = RIGHT;
	}
      break;
	  
    case DOWN:
      if (maze.item_at(x+8, y+28) != TileType::WALL)
	{
	  velocity_x = 0;
	  velocity_y = 2;
	  x = maze.snap_x(x+8);
	  current_animation = &pacman_down;
	  direction = DOWN;
	}
      break;

    case LEFT:
      if (maze.item_at(x-4, y+8) != TileType::WALL)
	{
	  velocity_x = -2;
	  velocity_y = 0;
	  y = maze.snap_y(y+8);
	  current_animation = &pacman_left;
	  direction = LEFT;
	}
      break;

    default:
      break;
    }
}

void
Pacman::handle_collisions(Maze &maze)
{
  // Right Side
  if (direction == RIGHT)
    {
      if (maze.item_at(x+16, y+8) == TileType::WALL)
	{
	  velocity_x = 0;
	  x = maze.snap_x(x);
	}
    }

  // Left Side
  if (direction == LEFT)
    {
      if (maze.item_at(x, y+8) == TileType::WALL)
	{
	  velocity_x = 0;
	  x = maze.snap_x(x+8);
	}
    }

  // Top Side
  if (direction == UP)
    {
      if (maze.item_at(x+8, y) == TileType::WALL)
	{
	  velocity_y = 0;
	  y = maze.snap_y(y+4);
	}
    }

  // Bottom Side
  if (direction == DOWN)
    {
      if (maze.item_at(x+8, y+16) == TileType::WALL)
	{
	  velocity_y = 0;
	  y = maze.snap_y(y+8);
	}
    }
}
