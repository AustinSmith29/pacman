#include "Maze.h"
#include <queue>
#include <stdexcept>
#include <iostream>

#define MIN(a, b) ((a) < (b)) ? (a) : (b)

int starting_state[][MAZE_WIDTH] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
  {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void
Maze::screen_to_grid(int &screen_x, int &screen_y)
{
  screen_x -= x_offset;
  screen_y -= y_offset;
  screen_x = screen_x / TILE_SIZE;
  screen_y = screen_y / TILE_SIZE;
}

void
Maze::grid_to_screen(int &grid_x, int &grid_y)
{
  grid_x *= TILE_SIZE;
  grid_y *= TILE_SIZE;
  grid_x += x_offset;
  grid_y += y_offset;
}

Maze::Maze()
{
  x_offset = 102;
  y_offset = 0;
  reset();
}

void
Maze::reset()
{
  tiles.clear();
  for (int y = 0; y < MAZE_HEIGHT; y++)
    {
      for (int x = 0; x < MAZE_WIDTH; x++)
	{
	  auto coord = std::make_pair(x, y);
	  switch(starting_state[y][x])
	    {
	    case TileType::OPEN:
	      tiles[coord] = Tile(x, y, TileType::OPEN);
	      break;
	    case TileType::WALL:
	      tiles[coord] = Tile(x, y, TileType::WALL);
	      break;
	    default:
	      break;
	    }
	}
    }
}

TileType
Maze::item_at(int x, int y)
{
  screen_to_grid(x, y);

  if (in_bounds(x, y))
    {
      auto coord = std::make_pair(x, y);
      return tiles[coord].type;
    }
  return TileType::UNKNOWN;
}

void
Maze::remove_at(int x, int y)
{
  if (!in_bounds(x, y))
    throw std::runtime_error("Maze::remove_at() out of bounds!");
  tiles[std::make_pair(x, y)].type = TileType::OPEN;
}

int
Maze::snap_x(int x)
{
  // ppConvert to grid position
  x -= x_offset;
  x /= TILE_SIZE;

  // Convert back to screen position (effectivley dropped overlap)
  x = x * TILE_SIZE;
  x += x_offset;
  return x;
}

int
Maze::snap_y(int y)
{
  y -= y_offset;
  y /= TILE_SIZE;
  y = y * TILE_SIZE;
  y += y_offset;
  return y;
}

void
Maze::draw(SDL_Renderer *renderer)
{
  for (int y = 0; y < MAZE_HEIGHT; y++)
    {
      for (int x = 0; x < MAZE_WIDTH; x++)
	{
	  SDL_Rect tile_bounds {
	      (x*TILE_SIZE) + x_offset,
	      (y*TILE_SIZE) + y_offset,
	      TILE_SIZE,
	      TILE_SIZE
	   };

	  if (tiles[std::make_pair(x, y)].type == TileType::WALL)
	    {
	      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	      SDL_RenderDrawRect(renderer, &tile_bounds);
	    }
	}
    }
}

bool
Maze::in_bounds(int x, int y)
{
  if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT)
    return true;
  return false;
}

typedef std::pair<int,int> cell;  // So I don't have to write this out every time.

/** Given a cell in the map, return vector of cells around
 *  that cell that are passable.
 **/
static std::vector<cell> get_neighbors(cell at, std::map<cell, Tile> &map)
{
  std::vector<cell> passable_cells;
  auto top = map.find(std::make_pair(at.first, at.second-1));
  auto right = map.find(std::make_pair(at.first+1, at.second));
  auto bottom = map.find(std::make_pair(at.first, at.second+1));
  auto left = map.find(std::make_pair(at.first-1, at.second));

  auto add_empty = [map, &passable_cells](decltype(top) cell_iter) -> void {
    if (cell_iter != map.end() && map.find(cell_iter->first)->second.type == TileType::OPEN)
      {
	passable_cells.push_back(cell_iter->first);
      }
  };

  add_empty(top);
  add_empty(right);
  add_empty(bottom);
  add_empty(left);
  
  return passable_cells;
}

static void DEBUG_PRINT_CELL(cell c)
{
  std::cout << "(" << c.first << ", " << c.second << ")\n";
}

std::stack<cell> get_path(std::map<cell, cell> parent, cell target, Maze &maze)
{
  std::stack<cell> path;
  auto current = parent.find(target);
  while (current != parent.end())
    {
      auto screen_coord_node = current->first;
      maze.grid_to_screen(screen_coord_node.first, screen_coord_node.second);
      path.push(screen_coord_node);
      current = parent.find(current->second);
    }
  return path;
}

std::stack<cell> dikstra(int target_x, int target_y,
				       int from_x, int from_y,
				       Maze &maze)
{
  const int INF = 9999999;
  maze.screen_to_grid(target_x, target_y);
  maze.screen_to_grid(from_x, from_y);

  // Initialize distances to infinity.
  std::map<cell, int> distances;
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      distances[std::make_pair(x, y)] = INF;
    }
  }

  auto current_cell = std::make_pair(from_x, from_y);
  auto target_cell  = std::make_pair(target_x, target_y);
  distances[current_cell] = 0;

  // Priority queue sorts cells by min distance in the distances map.
  auto cmp = [distances](cell a, cell b) -> bool {
    return distances.find(a)->second > distances.find(b)->second;
  };
  
  std::priority_queue<cell, std::vector<cell>, decltype(cmp)> nodes(cmp);
  std::vector<cell> visited;
  std::map<cell, cell> parent;
  nodes.push(current_cell);
  
  while (!nodes.empty())
    {
      auto current_cell = nodes.top();
      nodes.pop();
      visited.push_back(current_cell);
     
      auto neighbors = get_neighbors(current_cell, maze.tiles);
      for (auto cell : neighbors) {
        if (distances[current_cell] + 1 < distances[cell]) {
          distances[cell] = distances[current_cell] + 1;
	  parent[cell] = current_cell;
          nodes.push(cell);
        }
      }
    }
  std::stack<cell> path = get_path(parent, target_cell, maze);
  return path;
}
