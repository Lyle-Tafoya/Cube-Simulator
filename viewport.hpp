#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include <GLFW/glfw3.h>

class Viewport
{
  GLFWwindow *window;
  float xAngle;
  float yAngle;
  float zoom;

public:
  Viewport(int width, int height);
  ~Viewport();
  GLFWwindow *GetWindow();
  void ResizeWindow(int width, int height);
  void RotateCamera(float x, float y, float);
  void ZoomCamera(float zoom);
};

#endif
