#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class Viewport
{
  GLFWwindow *window;
  int width;
  int height;
  float xAngle;
  float yAngle;
  float zoom;

public:
  Viewport(int width, int height);
  ~Viewport();
  inline GLFWwindow *getWindow() const { return window; }
  void adjustCamera(float xAngle, float yAngle, float zoom);
  void resetCamera();
  void resizeWindow(int width, int height);
  void setCamera(float xAngle, float yAngle, float zoom);

  static void framebufferSizeCallback(GLFWwindow *window, int x, int y);
};

#endif
