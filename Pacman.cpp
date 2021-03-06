#include "Pacman.h"

void
Pacman::init(GraphicsLoader *loader)
{
  pacman_right = loader->load_animation("data/pacman_right.dat");
  pacman_up    = loader->load_animation("data/pacman_up.dat");
  pacman_left  = loader->load_animation("data/pacman_left.dat");
  pacman_down  = loader->load_animation("data/pacman_down.dat");
  pacman_die   = loader->load_animation("data/pacman_die.dat");
  current_animation = &pacman_left;

  reset();
}

void
Pacman::reset()
{
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
Pacman::tunnel_left()
{
  x = 550;
}

void
Pacman::tunnel_right()
{
  x = 100;
}

int Pacman::get_x() { return x; }
int Pacman::get_y() { return y; }
Direction Pacman::get_direction() { return direction; }

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
          switch_animation(*current_animation, pacman_up);
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
          switch_animation(*current_animation, pacman_right);
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
          switch_animation(*current_animation, pacman_down);
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
          switch_animation(*current_animation, pacman_left);
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
          pause_animation(*current_animation);
        }
    }

  // Left Side
  if (direction == LEFT)
    {
      if (maze.item_at(x, y+8) == TileType::WALL)
        {
          velocity_x = 0;
          x = maze.snap_x(x+8);
          pause_animation(*current_animation);
        }
    }

  // Top Side
  if (direction == UP)
    {
      if (maze.item_at(x+8, y) == TileType::WALL)
        {
          velocity_y = 0;
          y = maze.snap_y(y+4);
          pause_animation(*current_animation);
        }
    }

  // Bottom Side
  if (direction == DOWN)
    {
      if (maze.item_at(x+8, y+16) == TileType::WALL)
        {
          velocity_y = 0;
          pause_animation(*current_animation);
          y = maze.snap_y(y+8);
        }
    }
}

void
Pacman::die()
{
  switch_animation(*current_animation, pacman_die);
  current_animation = &pacman_die;
  if (is_animation_complete(*current_animation))
    {
      reset();
    }
}
