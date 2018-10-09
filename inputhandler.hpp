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
  static size_t sliceMod;

public:
  static void Init(Cube *cube, Viewport *viewport);
  static void Update(float deltaTime);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif
