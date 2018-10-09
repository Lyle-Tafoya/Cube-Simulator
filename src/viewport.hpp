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
  void adjustCamera(float xAngle, float yAngle, float zoom);
  void resetCamera();
  void resizeWindow(int width, int height);
  void setCamera(float xAngle, float yAngle, float zoom);
};

#endif
