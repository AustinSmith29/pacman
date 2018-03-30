#include "Graphics.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

void
GraphicsLoader::register_service(SDL_Renderer *renderer)
{
	this->renderer = renderer;
}

Sprite
GraphicsLoader::load_sprite(std::string filepath)
{
  if (!renderer)
    throw std::runtime_error("GraphicsLoader has not been registered!");

  // Retrieve texture if it has been loaded before.
  auto it = registry.find(filepath);
  if (it != registry.end())
    {
      auto texture = registry[filepath];
      Sprite new_sprite = std::make_unique<_Sprite>(texture);
      return new_sprite;
    }
  
  // Else load it for first time.
  SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());
  if (!surface)
    {
      std::cout << SDL_GetError() << std::endl;
      return nullptr;
    }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture)
    {
      std::cout << SDL_GetError() << std::endl;
      return nullptr;
    }
  
  SDL_FreeSurface(surface);
  registry[filepath] = std::shared_ptr<SDL_Texture>(texture,
						    [](SDL_Texture *t)
						    {
						      SDL_DestroyTexture(t);
						    });
  Sprite new_sprite = std::make_unique<_Sprite>(registry[filepath]);
  return new_sprite;
}

void draw_sprite(SDL_Renderer *renderer, Sprite &sprite, int x, int y)
{
	// NULL, NULL just to verify everything is working at first.
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	SDL_QueryTexture(sprite->texture.get(), NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, sprite->texture.get(), NULL, &rect);
}

Animation
GraphicsLoader::load_animation(std::string filepath)
{
	if (!renderer)
		throw std::runtime_error("GraphicsLoader has not been registered!");

	// Load animation parameters from file
	std::string sheetpath;
	int dx, dy, frames, ticks;
	parse_animation_file(filepath, sheetpath, dx, dy, frames, ticks);

	Sprite sheet = load_sprite(sheetpath);
	if (!sheet)
	  {
	    std::cout << "Could not create animation... spritesheet could not be loaded.\n";
	    return nullptr;
	  }
	Animation animation = make_animation_from_sheet(sheetpath, dx, dy, frames, ticks);
	if (!animation)
	  {
	    std::cout << "Could not create animation... " << SDL_GetError() << std::endl;
	    return nullptr;
	  }
	return animation;
}

Animation
GraphicsLoader::make_animation_from_sheet(std::string filepath,
		int dx, int dy, int frames, int ticks)
{
  Sprite sheet = load_sprite(filepath);
  if (!sheet)
    {
      std::cout << "Could not create animation... spritesheet did not load.\n";
      return nullptr;
    }

  Animation animation = std::make_unique<_Animation>();
  animation->sheet = std::move(sheet);
  animation->current_frame = 0;
  animation->paused = false;
  animation->ticks_per_frame = ticks;
  animation->current_tick = 0;

  int x, y;
  x = y = 0;
  for (auto i = 0; i < frames; i++)
    {
      SDL_Rect frame = {x, y, dx, dy};
      animation->frames.push_back(frame);
      x += dx;  // Only moves linearly to the right for next frame.
    }
  
  return animation;
}

void
GraphicsLoader::parse_animation_file(std::string filepath, std::string &sheet, 
			  int &dx, int &dy, int &frames, int &ticks)
{
  std::ifstream in(filepath);
  if (!in)
    {
      throw std::runtime_error("Could not open animation file: " + filepath);
    }
  std::string line;
  getline(in, line);
  std::istringstream record(line);
  record >> sheet;
  record >> dx;
  record >> dy;
  record >> frames;
  record >> ticks;
  in.close();
}

static
void draw_single_frame_animation(SDL_Renderer *renderer, Animation &a, int x, int y)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  // Hacky but just grab first width/height of first frame since frame size is uniform.
  if (a->frames.size() > 0)
    {
      rect.w = a->frames[0].w;
      rect.h = a->frames[0].h;
    }
  else
    {
      std::cout << "Warning: Animation has no frames.\n";
      rect.w = 0;
      rect.h = 0;
    }
  
  SDL_RenderCopy(renderer, a->sheet->texture.get(), &(a->frames[a->current_frame]), &rect);
}

void play_animation(SDL_Renderer *renderer, Animation &animation, int x, int y)
{
  if (!animation->paused)
    {
      draw_single_frame_animation(renderer, animation, x, y);
      animation->current_tick++;
      // Advance animation frame if timer goes off.
      if (animation->current_tick >= animation->ticks_per_frame)
	{
	  animation->current_frame++;
	  animation->current_tick = 0;
	}
      // Restart animation on completion.
      if (animation->current_frame >= animation->frames.size())
	{
	  animation->current_frame = 0;
	}
    }
  else
    {
      draw_single_frame_animation(renderer, animation, x, y);
    }
}

void switch_animation(Animation &old_anim, Animation &new_anim)
{
  pause_animation(old_anim);
  unpause_animation(new_anim);
}

void pause_animation(Animation &animation)
{
  animation->paused = true;
}

void unpause_animation(Animation &animation)
{
  animation->paused = false;
}
