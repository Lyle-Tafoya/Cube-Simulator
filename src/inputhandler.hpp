#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <GLFW/glfw3.h>
#include "cube.hpp"
#include "viewport.hpp"

class InputHandler
{
  static GLFWwindow *glfwWindow;
  static Viewport *viewport;
  static Cube *cube;
  static unsigned int sliceMod;

public:
  static void init(Cube *cube, Viewport *viewport);
  static void update(float deltaTime);
  static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif
