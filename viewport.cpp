#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "cube.hpp"
#include "viewport.hpp"

Viewport::Viewport(int width, int height)
        : xAngle(35.f), yAngle(-35.f), zoom(0.f)
{
  // Setup GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  window = glfwCreateWindow(width, height, "Cube Simulator", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Setup OpenGL
  glClearColor(0.6f, 0.6f, 0.6f, 0.6f);
  glClearDepth(1.f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  ResizeWindow(width, height);
}

Viewport::~Viewport()
{
  glfwTerminate();
}

GLFWwindow *Viewport::GetWindow()
{
  return window;
}

// Adjust the projection matrix to a new width/height
void Viewport::ResizeWindow(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.f, static_cast<float>(width) / static_cast<float>(height), 1.f, 10000.f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.f, 0.f, 237.f,
            0.f, 0.f, 0.f,
            0.f, 1.f, 0.f);

}

void Viewport::RotateCamera(float xAxis, float yAxis, float)
{
  xAngle += xAxis;
  yAngle += yAxis;
}
