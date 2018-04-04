#include "Maze.h"
#include <queue>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstdlib> // for rand()
#include <ctime>   // ditto

int starting_state[][MAZE_WIDTH] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 3, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 3, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 5, 5, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {4, 4, 4, 4, 4, 4, 0, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 0, 4, 4, 4, 4, 4, 4},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 1},
  {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
  {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
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
Maze::init(GraphicsLoader *loader)
{
  tile_sprites[TileType::DOT]    = loader->load_sprite("assets/imgs/dot.bmp");
  tile_sprites[TileType::BIGDOT] = loader->load_sprite("assets/imgs/powerdot.bmp");
  tile_sprites[TileType::WALL]   = loader->load_sprite("assets/imgs/wall.bmp");
  tile_sprites[TileType::GATE]   = loader->load_sprite("assets/imgs/gate.bmp");
}

void
Maze::reset()
{
  srand(time(NULL));
  tiles.clear();
  neighbors.clear();
  for (int y = 0; y < MAZE_HEIGHT; y++)
    {
      for (int x = 0; x < MAZE_WIDTH; x++)
        {
          auto coord = std::make_pair(x, y);
          switch(starting_state[y][x])
            {
            case TileType::DOT:
              tiles[coord] = Tile(x, y, TileType::DOT);
              break;
            case TileType::WALL:
              tiles[coord] = Tile(x, y, TileType::WALL);
              break;
            case TileType::BIGDOT:
              tiles[coord] = Tile(x, y, TileType::BIGDOT);
              break;
            case TileType::GATE:
              tiles[coord] = Tile(x, y, TileType::GATE);
              break;
            default:
              tiles[coord] = Tile(x, y, TileType::OPEN);
              break;
            }
        }
    }
  setup_neighbors();
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
  screen_to_grid(x, y);
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

cell
Maze::random_cell()
{
  int x = rand() % MAZE_WIDTH;
  int y = rand() % MAZE_HEIGHT;
  return std::make_pair(x, y);
}

void
Maze::draw(SDL_Renderer *renderer)
{
  for (int y = 0; y < MAZE_HEIGHT; y++)
    {
      for (int x = 0; x < MAZE_WIDTH; x++)
        {
          /*
          SDL_Rect tile_bounds {
            (x*TILE_SIZE) + x_offset,
              (y*TILE_SIZE) + y_offset,
              TILE_SIZE,
              TILE_SIZE
              };
          */
          int screen_x = (x * TILE_SIZE) + x_offset;
          int screen_y = (y * TILE_SIZE) + y_offset;
          auto c = std::make_pair(x,y);

          if (tile_sprites.find(tiles[c].type) != tile_sprites.end())
            {
              draw_sprite(renderer, tile_sprites[tiles[c].type],
                          screen_x, screen_y);
            }
          else
            {
              continue;
            }
          /*
          if (tiles[c].type == TileType::WALL)
            {
              SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
              SDL_RenderDrawRect(renderer, &tile_bounds);
            }
          if (tiles[c].type == TileType::DOT)
            {
              SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
              // Obviously a temporary solution until tiles.
              SDL_Rect dot {
                tile_bounds.x + 6,
                  tile_bounds.y + 6,
                  4, 4
                  };
		  
              SDL_RenderDrawRect(renderer, &dot);
            }
          if (tiles[c].type == TileType::BIGDOT)
            {
              SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
              SDL_Rect bigdot {
                tile_bounds.x + 4,
                  tile_bounds.y + 4,
                  8, 8
                  };
              SDL_RenderDrawRect(renderer, &bigdot);
            }
          */
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

void
Maze::setup_neighbors()
{
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      cell c = std::make_pair(x, y);
      neighbors[c] = get_neighbors(c);
    }
  }
}

std::vector<cell>
Maze::get_neighbors(cell at)
{
  std::vector<cell> passable_cells;
  auto top = tiles.find(std::make_pair(at.first, at.second-1));
  auto right = tiles.find(std::make_pair(at.first+1, at.second));
  auto bottom = tiles.find(std::make_pair(at.first, at.second+1));
  auto left = tiles.find(std::make_pair(at.first-1, at.second));

  auto add_empty = [this, &passable_cells](decltype(top) cell_iter) -> void {
    if (cell_iter != tiles.end() && tiles.find(cell_iter->first)->second.type != TileType::WALL)
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


/**
 * Currently being used as the heuristic to find next cell in Dijikstra.
 *
 * @param a,b the two cells which the distance will be computed
 * @return the manhattan distance between cells a and b
 **/
static int manhattan_distance(cell a, cell b)
{
  int dx = b.first - a.first;
  int dy = b.second - a.second;
  return abs(dx) + abs(dy);
}

/**
 * Take candidate vector of cells and pick the cell from that vector that has
 * the best heuristic value that has not been already visited by Dijikstra.
 *
 * @param candidates the neighboring cells
 * @param visited    cells that have already been visited
 * @param target     the destination cell
 **/
static cell apply_heuristic(std::vector<cell> candidates, std::vector<cell> visited, cell target)
{
  // TODO: Can I shorten this function?
  int best_index = -1;
  int small_distance = -1;

  for (unsigned int i = 0; i < candidates.size(); i++) {
    if (std::find(visited.begin(), visited.end(), candidates[i]) != visited.end())
      {
        continue;
      }
    int d = manhattan_distance(candidates[i], target);
    if (d < small_distance || small_distance == -1) {
      small_distance = d;
      best_index = i;
    }
  }
  return candidates[best_index];
}

std::stack<cell> dikstra(int target_x, int target_y,
			 int from_x, int from_y,
			 Maze &maze)
{
  maze.screen_to_grid(target_x, target_y);
  maze.screen_to_grid(from_x, from_y);
  auto current_cell = std::make_pair(from_x, from_y);
  auto target_cell  = std::make_pair(target_x, target_y);
  if (current_cell == target_cell)
    {
      std::stack<cell> empty;
      return empty;
    }

  // Initialize distances to infinity
  const int INF = 9999999;
  std::map<cell, int> distances;
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      distances[std::make_pair(x,y)] = INF;
    }
  }
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
      if (current_cell == target_cell)
        break;

      // Select next best cell.
      auto candidates = maze.neighbors[current_cell];
      // Hack for right now... look up A* to see how to deal with this.
      if (candidates.size() == 0)
        break;
      cell next_cell = apply_heuristic(candidates, visited, target_cell);
      parent[next_cell] = current_cell;
      
      nodes.push(next_cell);
    }
  // TEST: change target_cell to last visited cell since this isn't really a true dijikstra algorithm,
  // and we are essentially just getting a single next closest square to target, then the last visited
  // cell would be the last open cell to target. This is useful for Pacman because when calculating
  // ghost target squares, many times the target lands on a wall or outside of the map boundry.
  // This method just tries to get as close to that target square as possible, and makes programming
  // the ghosts much easier at the expense of the pathfinding being sub-optimal, which I think is okay.
  std::stack<cell> path = get_path(parent, visited.back(), maze);
  return path;
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
