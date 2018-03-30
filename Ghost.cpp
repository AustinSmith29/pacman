#include "Ghost.h"
#include <iostream>

void
ChaseFunction::red_ghost(Ghost &ghost, Maze &maze, AIState &state)
{
  ghost.go_to(state.pacman_x, state.pacman_y, maze);
}

void
ChaseFunction::blue_ghost(Ghost &ghost, Maze &maze, AIState &state)
{
  int target_x, target_y;
  if (state.pacman_direction == UP)
    {
      target_x = state.pacman_x;
      target_y = state.pacman_y - (4 * TILE_SIZE);
    }
  else if (state.pacman_direction == RIGHT)
    {
      target_x = state.pacman_x + (4 * TILE_SIZE);
      target_y = state.pacman_y;
    }
  else if (state.pacman_direction == DOWN)
    {
      target_x = state.pacman_x;
      target_y = state.pacman_y + (4 * TILE_SIZE);
    }
  else
    {
      target_x = state.pacman_x - (4 * TILE_SIZE);
      target_y = state.pacman_y;
    }
  ghost.go_to(target_x, target_y, maze);
}

void
ChaseFunction::pink_ghost(Ghost &ghost, Maze &maze, AIState &state)
{
  int target_x, target_y;
  if (state.pacman_direction == UP)
    {
      target_x = state.pacman_x;
      target_y = state.pacman_y + (4 * TILE_SIZE);
    }
  else if (state.pacman_direction == RIGHT)
    {
      target_x = state.pacman_x - (4 * TILE_SIZE);
      target_y = state.pacman_y;
    }
  else if (state.pacman_direction == DOWN)
    {
      target_x = state.pacman_x;
      target_y = state.pacman_y - (4 * TILE_SIZE);
    }
  else
    {
      target_x = state.pacman_x + (4 * TILE_SIZE);
      target_y = state.pacman_y;
    }
  ghost.go_to(target_x, target_y, maze);
}

void
ChaseFunction::orange_ghost(Ghost &ghost, Maze &maze, AIState &state)
{
  int dx = ghost.x - state.pacman_x;
  int dy = ghost.y - state.pacman_y;
  int distance = (int)sqrt(pow(dx, 2) + pow(dy, 2));

  if (distance > 8)
    {
      ghost.go_to(state.pacman_x, state.pacman_y, maze);
    }
  else
    {
      ghost.go_to(ghost.scatter_x, ghost.scatter_y, maze);
    }
}

void
Ghost::init(GraphicsLoader *loader, int x, int y,
            std::function<void(Ghost&, Maze&, AIState&)> chase,
            int scat_x, int scat_y)
{
  sprite = loader->load_sprite("ghost.bmp");
  this->x = x;
  this->y = y;
  scatter_x = scat_x;
  scatter_y = scat_y;
  speed = 2;
  current_speed = speed;
  current_state = GhostState::CHASE;
  has_path = false;
  path_ticks = 0;
  path_timer = 100;
  this->chase = chase;
}

void
Ghost::update(Maze &maze, AIState &state)
{
  int target_x = 0;
  int target_y = 0;
  switch(current_state)
    {
    case CHASE:
      if (!has_path || path_ticks >= path_timer)
        {
          chase(*this, maze, state);
        }
      break;
    case SCATTER:
      if (!has_path || path_ticks >= path_timer)
        {
          go_to(scatter_x, scatter_y, maze);
        }
      break;
    case SCARED:
      if (!has_path || path_ticks >= path_timer)
        {
          cell random_cell = maze.random_cell();
          go_to(random_cell.first, random_cell.second, maze);
        }
      break;
    }

  if (!path.empty())
    {
      auto coord = path.top();
      target_x = coord.first;
      target_y = coord.second;
      if (x == target_x && y == target_y)
        {
          path.pop();
        }
      int dx = target_x - x;
      int dy = target_y - y;
      if (dx < 0)
        x -= current_speed;
      if (dx > 0)
        x += current_speed;
      if (dy < 0)
        y -= current_speed;
      if (dy > 0)
        y += current_speed;
    }
  else
    {
      has_path = false;
    } 
  path_ticks++;
}

void
Ghost::go_to(int tx, int ty, Maze &maze)
{
  path = dikstra(tx, ty, x, y, maze);
  has_path = true;
  path_ticks = 0;
}

void
Ghost::set_state(GhostState new_state)
{
  if (new_state == GhostState::SCARED)
    current_speed = SCARED_SPEED;
  else
    current_speed = speed;

  current_state = new_state;
}

void
Ghost::draw(SDL_Renderer *renderer)
{
  draw_sprite(renderer, sprite, x, y);
}
