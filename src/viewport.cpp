#include "viewport.hpp"

Viewport::Viewport(int width, int height)
        : width(width), height(height), xAngle(35.f), yAngle(-35.f), zoom(0.f)
{
  // Setup GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  window = glfwCreateWindow(width, height, "Cube Simulator", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetFramebufferSizeCallback(window, Viewport::framebufferSizeCallback);

  // Setup OpenGL
  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  glClearDepth(1.f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  Viewport::framebufferSizeCallback(nullptr, width, height);
  setCamera(xAngle, yAngle, zoom);
}

Viewport::~Viewport()
{
  glfwTerminate();
}

void Viewport::adjustCamera(float xAngle, float yAngle, float zoom)
{
  setCamera(this->xAngle+xAngle, this->yAngle+yAngle, this->zoom+zoom);
}

void Viewport::framebufferSizeCallback(GLFWwindow *, int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.f, static_cast<float>(width) / static_cast<float>(height), 1.f, 1000000.f);
}

void Viewport::resetCamera()
{
  setCamera(35.f, -35.f, -0.f);
}

void Viewport::setCamera(float xAngle, float yAngle, float zoom)
{
  this->xAngle = xAngle;
  this->yAngle = yAngle;
  this->zoom = zoom;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.f, 0.f, 500.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
  glTranslatef(0.f, 0.f, zoom);
  glRotatef(xAngle, 1.f, 0.f, 0.f);
  glRotatef(yAngle, 0.f, 1.f, 0.f);
}
