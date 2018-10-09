#include "cube.hpp"
#include "inputhandler.hpp"
#include "viewport.hpp"

Cube *InputHandler::cube = nullptr;
GLFWwindow *InputHandler::glfwWindow = nullptr;
Viewport *InputHandler::viewport = nullptr;
size_t InputHandler::sliceMod = 0;

void InputHandler::init(Cube *cube, Viewport *viewport)
{
  InputHandler::viewport = viewport;
  glfwWindow = viewport->getWindow();
  InputHandler::cube = cube;
  glfwSetKeyCallback(glfwWindow, keyboardCallback);
}

void InputHandler::update(float deltaTime)
{
  float elapsedTime = deltaTime*1000.f;

  // Camera rotation
  float xAngle = 0;
  float yAngle = 0;
  if(glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    xAngle += elapsedTime / 5000.f * 360.f;
  }
  if(glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
  {
    xAngle -= elapsedTime / 5000.f * 360.f;
  }
  if(glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    yAngle -= elapsedTime / 3000.f * 360.f;
  }
  if(glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    yAngle += elapsedTime / 3000.f * 360.f;
  }
  viewport->rotateCamera(xAngle, yAngle, 0.f);

  // Zoom in and out
  float zoom = 0;
  if(glfwGetKey(glfwWindow, GLFW_KEY_EQUAL) == GLFW_PRESS)
  {
    zoom += elapsedTime / 3000.f * cube->getSize() * 100.f;
  }
  if(glfwGetKey(glfwWindow, GLFW_KEY_MINUS) == GLFW_PRESS)
  {
    zoom -= elapsedTime / 3000.f * cube->getSize() * 100.f;
  }
  viewport->zoomCamera(zoom);

  glTranslatef(0.f, 0.f, zoom);
  glRotatef(xAngle, 1.f, 0.f, 0.f);
  glRotatef(yAngle, 0.f, 1.f, 0.f);
}

void InputHandler::keyboardCallback(GLFWwindow *, int key, int, int action, int)
{
  if(action != GLFW_PRESS) { return; }
  switch(key)
  {
    // Scramble the cube
    case GLFW_KEY_TAB:
      cube->scramble(cube->getSize() * 10);
      break;

      // Turn the front face
    case GLFW_KEY_H:
      cube->twist(cube->getSize() - 1 - sliceMod, Cube::Axis::Z);
      break;
    case GLFW_KEY_G:
      cube->twist(cube->getSize() - 1 - sliceMod, -Cube::Axis::Z);
      break;

      // Turn the top face
    case GLFW_KEY_J:
      cube->twist(sliceMod, Cube::Axis::Y);
      break;
    case GLFW_KEY_F:
      cube->twist(sliceMod, -Cube::Axis::Y);
      break;

      // Turn the left face
    case GLFW_KEY_D:
      cube->twist(sliceMod, -Cube::Axis::X);
      break;
    case GLFW_KEY_E:
      cube->twist(sliceMod, Cube::Axis::X);
      break;

      // Turn the right face
    case GLFW_KEY_K:
      cube->twist(cube->getSize()-1-sliceMod, -Cube::Axis::X);
      break;
    case GLFW_KEY_I:
      cube->twist(cube->getSize()-1-sliceMod, Cube::Axis::X);
      break;

      // Turn the bottom face
    case GLFW_KEY_L:
      cube->twist(cube->getSize()-1-sliceMod, Cube::Axis::Y);
      break;
    case GLFW_KEY_S:
      cube->twist(cube->getSize()-1-sliceMod, -Cube::Axis::Y);
      break;

      // Turn the back face
    case GLFW_KEY_O:
      cube->twist(sliceMod, Cube::Axis::Z);
      break;
    case GLFW_KEY_W:
      cube->twist(sliceMod, -Cube::Axis::Z);
      break;

    // Turn the whole cube on the x-axis
    case GLFW_KEY_B:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, -Cube::Axis::X);
      }
      break;
    case GLFW_KEY_Y:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, Cube::Axis::X);
      }
      break;

      // Turn the whole cube on the y-axis
    case GLFW_KEY_A:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, -Cube::Axis::Y);
      }
      break;
    case GLFW_KEY_SEMICOLON:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, Cube::Axis::Y);
      }
      break;

      // Turn the whole cube on the z-axis
    case GLFW_KEY_P:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, Cube::Axis::Z);
      }
      break;
    case GLFW_KEY_Q:
      for(unsigned int layer = 0; layer < cube->getSize(); ++layer)
      {
        cube->twist(layer, -Cube::Axis::Z);
      }
      break;

      // Change the target slice
    case GLFW_KEY_LEFT_SHIFT:
      if(sliceMod > 0)
      {
        --sliceMod;
      }
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      if(sliceMod < cube->getSize() - 1)
      {
        ++sliceMod;
      }
      break;

      // Choose a new cube size
    case GLFW_KEY_1:
      cube->resize(sliceMod*10+1);
      break;
    case GLFW_KEY_2:
      cube->resize(sliceMod*10+2);
      break;
    case GLFW_KEY_3:
      cube->resize(sliceMod*10+3);
      break;
    case GLFW_KEY_4:
      cube->resize(sliceMod*10+4);
      break;
    case GLFW_KEY_5:
      cube->resize(sliceMod*10+5);
      break;
    case GLFW_KEY_6:
      cube->resize(sliceMod*10+6);
      break;
    case GLFW_KEY_7:
      cube->resize(sliceMod*10+7);
      break;
    case GLFW_KEY_8:
      cube->resize(sliceMod*10+8);
      break;
    case GLFW_KEY_9:
      cube->resize(sliceMod*10+9);
      break;
    case GLFW_KEY_0:
      cube->resize(sliceMod*10+10);
      break;

      // Default statement so compiler will stop whining about all enum cases not being handled
    default:
      break;
  }
}
