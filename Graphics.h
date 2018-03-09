/** @file Graphics.h
 *  @brief Definitions used to handle graphics. 
 *
 *  This contains the objects that will be used to draw sprites and animations 
 *  to the screen.
 *
 *  @author Austin Smith
 *  @bug No known bugs.
 **/

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

/** 
 * _Sprite
 * A _Sprite wraps the functionality of a SDL_Texture. They are the
 * images that can be drawn on the screen.
 **/

struct _Sprite
{
  //  _Sprite() = default;
  _Sprite(std::shared_ptr<SDL_Texture> texture) { this->texture = texture; }

  std::shared_ptr<SDL_Texture> texture;  /// texture resource this sprite uses
};

typedef std::unique_ptr<_Sprite> Sprite;  // make it easier to pass around created _Sprite

/**
 * _Animation
 * An _Animation is exactly what it sounds like. It stores
 * information that allows sprites to be animated.
 **/

struct _Animation
{
  Sprite sheet;                        /// spritesheet the animation utilizes
  std::vector<SDL_Rect> frames;            /// clippings on sprite sheet of frames
  unsigned int current_frame;          /// current_frame of the animation
  unsigned int ticks_per_frame;        /// controls animation playing speed
  unsigned int current_tick;           /// if > ticks_per_frame, we move to next frame
  bool paused;                         /// determines if we play animation
};

typedef std::unique_ptr<_Animation> Animation;  // make it easier to pass around created _Animation

/**
 * GraphicsLoader
 * GraphicsLoader loads and returns created Sprite and Animation objects while
 * keeping track of SDL_Textures' that have already been loaded so the same
 * resource does not get loaded into memory more than once.
 **/

class GraphicsLoader
{
public:
  /**
   * Register this service with the renderer so that it can be used.
   * GraphicsLoader instances can not be used without first registering.
   * @param renderer the SDL_Renderer this GraphicsLoader will use.
   **/
  void      register_service(SDL_Renderer *renderer);

  /**
   * Loads an image and returns a created Sprite.
   * @param filepath the filepath to the image file
   * @return the created Sprite
   **/
  Sprite    load_sprite(std::string filepath);

  /**
   * Loads an image and returns a created Animation.
   * @param filepath the filepath to the animation datafile.
   * @return the created Animation.
   **/
  Animation load_animation(std::string filepath);

private:
  std::map<std::string, std::shared_ptr<SDL_Texture>>      registry;
  SDL_Renderer                                             *renderer;

  /**
   * Creates an animation by splitting up a spritesheet.
   * Assumes spritesheet has frames moving from left to right.
   * @param filepath filepath to the spritesheet file.
   * @param dx the width of each frame of the animation.
   * @param dy the height of each frame of the animation.
   * @param frames the number of frames in the animation
   * @param ticks the number of ticks per frame of the animation
   * @return the created Animation
   **/
  Animation make_animation_from_sheet(std::string filepath,
	    			         int dx, int dy,
				         int frames, int ticks);

  /**
   * Reads in parameters from a file that describe an animation.
   * This is a helper function to make_animation_from_sheet. The
   * parameters to this function correspond to the parameters
   * in make_animation_from_sheet. They are references to mimic
   * returning multiple values.
   *
   * Animation data files take the form:
   * sheetpath dx dy frames ticks
   **/
  void      parse_animation_file(std::string filepath,
        		         std::string &sheetpath,  /// filepath to spritesheet
			         int         &dx,         /// width of each frame
			         int         &dy,         /// height of each frame
			         int         &frames,     /// number of animation frames
			         int         &ticks);     /// number of ticks per frame
};

void draw_sprite(SDL_Renderer *renderer, Sprite &sprite, int x, int y);

void play_animation(SDL_Renderer *renderer, Animation &animation, int x, int y);
void pause_animation(Animation &animation);
#endif
