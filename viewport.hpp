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
  inline GLFWwindow *getWindow() { return window; }
  void resizeWindow(int width, int height);
  void rotateCamera(float x, float y, float);
  void zoomCamera(float zoom);
};

#endif
