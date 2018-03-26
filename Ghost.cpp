#include "Ghost.h"
#include <iostream>

void
Ghost::init(GraphicsLoader *loader)
{
  sprite = loader->load_sprite("ghost.bmp");
  x = 318;
  y = 272;
  target_x = target_y = 0;
  current_state = GhostState::SCATTER;
  has_path = false;
}

void
Ghost::update(Maze &maze)
{
  switch(current_state)
    {
    case CHASE:
      if (!path.empty())
	{
	  auto coord = path.top();
	  path.pop();
	  target_x = coord.first;
	  target_y = coord.second;
	}
      break;
    case SCATTER:
      if (!has_path)
	{
	  go_to(302, 16, maze);
	  has_path = true;
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
	    x -= 2;
	  if (dx > 0)
	    x += 2;
	  if (dy < 0)
	    y -= 2;
	  if (dy > 0)
	    y += 2;
	}
      else
	{
	  go_to(225, 16, maze);
	}
      break;
    case SCARED:
      break;
    }
}

void
Ghost::go_to(int tx, int ty, Maze &maze)
{
  target_x = tx;
  target_y = ty;
  path = dikstra(tx, ty, x, y, maze);
}

void
Ghost::set_state(GhostState new_state)
{
  current_state = new_state;
}

void
Ghost::draw(SDL_Renderer *renderer)
{
  draw_sprite(renderer, sprite, x, y);
}
