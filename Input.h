/** @file Input.h
 *  @brief Input class handles keypresses
 *
 * This contains the Input class which handles
 * keyboard input events and eventually other types
 * of input such as joystick and mouse events.
 *
 * @author Austin Smith
 * @bugs No known bugs.
 **/

#ifndef _INPUT_H
#define _INPUT_H

#include <SDL2/SDL.h>
#include <string>
#include <map>

/**
 * Input
 * Maps input events to user-defined actions.
 **/

class Input
{
 public:
  /**
   * Associates keycode with action.
   * @param code the SDL_Keycode
   * @param action the user-defined action the code gets bound to
   **/
  void bind(SDL_Keycode code, std::string action);

  /**
   * Retrieves action string associated with the event.
   * @param &e the SDL_Event
   * @return the corresponding action. If no match, this returns "NONE"
   **/
  std::string get_action(SDL_Event &e);

 private:
  std::map<SDL_Keycode, std::string> input_map;
};
#endif
