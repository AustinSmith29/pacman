#include "Ghost.h"
#include <iostream>
#include <stdexcept>

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

Ghost::Ghost(int scatter_x, int scatter_y,
             std::function<void(Ghost&, Maze&, AIState&)> chase)
  : scatter_timer(100), path_timer(100), ghost_house_timer(0), scared_timer(500)
{
  this->scatter_x = scatter_x;
  this->scatter_y = scatter_y;
  this->chase = chase;
}

void
Ghost::init(GraphicsLoader *loader, std::string sprite_filepath, int x, int y,
            int house_time)
{
  this->x = x;
  this->y = y;
  this->sprite = loader->load_sprite(sprite_filepath);
  this->scared_sprite = loader->load_sprite("assets/imgs/scared_ghost.bmp");
  this->eyes_sprite = loader->load_sprite("assets/imgs/ghost_eyes.bmp");
  this->speed = 2;
  this->current_speed = speed;
  this->has_path = false;
  current_sprite = &this->sprite;
  can_pass_gate = false;
  ghost_house_timer.set(house_time);
  set_state(GhostState::HOUSE);
}

int
Ghost::get_x()
{
  return x;
}

int
Ghost::get_y()
{
  return y;
}

bool
Ghost::is_eatable()
{
  return current_state == GhostState::SCARED;
}

void
Ghost::update(Maze &maze, AIState &state)
{
  state_logic(maze, state);
  follow_path();
}

void
Ghost::chase_state(Maze &maze, AIState &state)
{
  if (!has_path || path_timer.finished())
    {
      chase(*this, maze, state);
    }
}

void
Ghost::scatter_state(Maze &maze, AIState &state)
{
  if (scatter_timer.finished())
    {
      std::cout << "Scatter Timer expired!\n";
      set_state(GhostState::CHASE);
    }
  if (!has_path || path_timer.finished())
    {
      go_to(scatter_x, scatter_y, maze);
    }
  scatter_timer.tick();
}

void
Ghost::scared_state(Maze &maze, AIState &state)
{
  if (scared_timer.finished())
    {
      std::cout << "Scared Timer expired!\n";
      set_state(GhostState::CHASE);
    }
  if (!has_path || path_timer.finished())
    {
      cell random_cell = maze.random_cell();
      go_to(random_cell.first, random_cell.second, maze);
    }
  scared_timer.tick();
}

void
Ghost::eaten_state(Maze &maze, AIState &state)
{
  set_state(GhostState::RETURN);
  go_to(318, 224, maze, true);
}

void
Ghost::return_state(Maze &maze, AIState &state)
{
  if (!has_path)
    {
      set_state(GhostState::HOUSE);
      ghost_house_timer.set(200);
      ghost_house_timer.reset();
    }
}

void
Ghost::house_state(Maze &maze, AIState &state)
{
  if (ghost_house_timer.finished())
    {
      std::cout << "Ghost House Timer expired!\n";
      const int MAZE_EXIT_X = 326;
      const int MAZE_EXIT_Y = 176;
      can_pass_gate = true;
      set_state(GhostState::LEAVE_HOUSE);
      go_to(MAZE_EXIT_X, MAZE_EXIT_Y, maze, can_pass_gate);
    }
  else
    {
      ghost_house_timer.tick();
    }
}

void
Ghost::leave_house_state(Maze &maze, AIState &state)
{
  // Finished going to maze exit
  if (!has_path)
    {
      can_pass_gate = false;
      set_state(GhostState::SCATTER);
    }
}

void
Ghost::follow_path()
{
  if (!path.empty())
    {
      auto coord = path.top();
      int target_x = coord.first;
      int target_y = coord.second;
      // Following hack solves "jittery" problem that often occurs on state
      // transitions. The ghost jumps back and forth from point to point because
      // the position never quite equals target coords. Ex: Target_x = 10,
      // and x = 9 but speed = 2... therefore after update x will be at 11 so it will
      // jump back, only to jump back yet again. Rinse Recycle Repeat.
      // Maybe fix by checking the grid cell... but then we will have to check for
      // intersections so ghosts don't change direction prematurely.
      if (abs(x - target_x) <= current_speed && abs(y - target_y) <= current_speed) 
        {
          path.pop();
        }
      int dx = target_x - x;
      int dy = target_y - y;
      if (dx < 0)
        x -= current_speed;
      else if (dx > 0)
        x += current_speed;
      else if (dy < 0)
        y -= current_speed;
      else if (dy > 0)
        y += current_speed;
    }
  else
    {
      has_path = false;
    } 
  path_timer.tick();
}

void
Ghost::go_to(int tx, int ty, Maze &maze, bool can_pass_gate)
{
  path = dikstra(tx, ty, x, y, maze, can_pass_gate);
  has_path = true;
  path_timer.reset();
}

void
Ghost::set_state(GhostState new_state)
{
  using namespace std::placeholders;
  switch(new_state)
    {
    case SCATTER:
      current_speed = speed;
      current_sprite = &sprite;
      scatter_timer.reset();
      state_logic = std::bind(&Ghost::scatter_state, this, _1, _2);
      break;

    case CHASE:
      current_speed = speed;
      current_sprite = &sprite;
      state_logic = std::bind(&Ghost::chase_state, this, _1, _2);
      break;

    case SCARED:
      current_speed = SCARED_SPEED;
      current_sprite = &scared_sprite;
      scared_timer.reset();
      state_logic = std::bind(&Ghost::scared_state, this, _1, _2);
      break;

    case EATEN: 
      state_logic = std::bind(&Ghost::eaten_state, this, _1, _2);
      break;

    case RETURN:
      current_speed = RETURN_SPEED;
      current_sprite = &eyes_sprite;
      state_logic = std::bind(&Ghost::return_state, this, _1, _2);
      break;

    case HOUSE:
      current_sprite = &sprite;
      ghost_house_timer.reset();
      state_logic = std::bind(&Ghost::house_state, this, _1, _2);
      break;

    case LEAVE_HOUSE:
      current_speed = SCARED_SPEED;
      state_logic = std::bind(&Ghost::leave_house_state, this, _1, _2);
      break;

    default:
      std::cout << "UNKNOWN STATE\n";
      break;
    }
  current_state = new_state;
  has_path = false;
}

void
Ghost::draw(SDL_Renderer *renderer)
{
  draw_sprite(renderer, *current_sprite, x, y);
}

std::unique_ptr<Ghost> ghost_factory(GraphicsLoader *loader, GhostType type)
{
  std::unique_ptr<Ghost> ghost;
  switch (type)
    {
    case RED:
      {
        ghost = std::make_unique<Ghost>(550, 16, ChaseFunction::red_ghost);
        ghost->init(loader, "assets/imgs/red_ghost.bmp", 318, 192, 0);
        ghost->set_state(GhostState::SCATTER); // red ghost starts outside ghosthouse
        return ghost;
      }
    case BLUE:
      {
        ghost = std::make_unique<Ghost>(550, 480, ChaseFunction::blue_ghost);
        ghost->init(loader, "assets/imgs/blue_ghost.bmp", 295, 224, 50);
        return ghost;
      }
    case ORANGE:
      {
        ghost = std::make_unique<Ghost>(118, 480, ChaseFunction::orange_ghost);
        ghost->init(loader, "assets/imgs/orange_ghost.bmp", 318, 224, 100);
        return ghost;
      }
    case PINK:
      {
        ghost = std::make_unique<Ghost>(118, 16, ChaseFunction::pink_ghost);
        ghost->init(loader, "assets/imgs/pink_ghost.bmp", 345, 224, 150);
        return ghost;
      }
    default:
      throw std::runtime_error("Unknown ghost type.");
    }
  return nullptr;
}
