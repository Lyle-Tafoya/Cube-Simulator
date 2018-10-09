#include "cube.hpp"
#include "inputhandler.hpp"
#include "viewport.hpp"

Cube *InputHandler::cube = nullptr;
GLFWwindow *InputHandler::glfwWindow = nullptr;
Viewport *InputHandler::viewport = nullptr;
size_t InputHandler::sliceMod = 0;

void InputHandler::Init(Cube *cube, Viewport *viewport)
{
  InputHandler::viewport = viewport;
  glfwWindow = viewport->GetWindow();
  InputHandler::cube = cube;
  glfwSetKeyCallback(viewport->GetWindow(),KeyCallback);
}

void InputHandler::Update(float deltaTime)
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
  viewport->RotateCamera(xAngle, yAngle, 0.f);

  // Zoom in and out
  float zoom = 0;
  if(glfwGetKey(glfwWindow, GLFW_KEY_EQUAL) == GLFW_PRESS)
    zoom += elapsedTime / 3000.f * cube->CUBIES_PER_EDGE * 100.f;
  if(glfwGetKey(glfwWindow, GLFW_KEY_MINUS) == GLFW_PRESS)
    zoom -= elapsedTime / 3000.f * cube->CUBIES_PER_EDGE * 100.f;

  glTranslatef(0.f, 0.f, zoom);
  glRotatef(xAngle, 1.f, 0.f, 0.f);
  glRotatef(yAngle, 0.f, 1.f, 0.f);
}

// Get all input not related to camera control
void InputHandler::KeyCallback(GLFWwindow *, int key, int, int action, int)
{
  if(action == GLFW_PRESS)
  {
    switch(key)
    {
      // Scramble the cube
      case GLFW_KEY_TAB:
        cube->Scramble(cube->CUBIES_PER_EDGE * 10);
        break;

        // Turn the front face
      case GLFW_KEY_H:
        cube->Twist(cube->CUBIES_PER_EDGE - 1 - sliceMod, AXIS_Z);
        break;
      case GLFW_KEY_G:
        cube->Twist(cube->CUBIES_PER_EDGE - 1 - sliceMod, -AXIS_Z);
        break;

        // Turn the top face
      case GLFW_KEY_J:
        cube->Twist(sliceMod, AXIS_Y);
        break;
      case GLFW_KEY_F:
        cube->Twist(sliceMod, -AXIS_Y);
        break;

        // Turn the left face
      case GLFW_KEY_D:
        cube->Twist(sliceMod, -AXIS_X);
        break;
      case GLFW_KEY_E:
        cube->Twist(sliceMod, AXIS_X);
        break;

        // Turn the right face
      case GLFW_KEY_K:
        cube->Twist(cube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_X);
        break;
      case GLFW_KEY_I:
        cube->Twist(cube->CUBIES_PER_EDGE-1-sliceMod, AXIS_X);
        break;

        // Turn the bottom face
      case GLFW_KEY_L:
        cube->Twist(cube->CUBIES_PER_EDGE-1-sliceMod, AXIS_Y);
        break;
      case GLFW_KEY_S:
        cube->Twist(cube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_Y);
        break;

        // Turn the back face
      case GLFW_KEY_O:
        cube->Twist(sliceMod, AXIS_Z);
        break;
      case GLFW_KEY_W:
        cube->Twist(sliceMod, -AXIS_Z);
        break;

      // Turn the whole cube on the x-axis
      case GLFW_KEY_B:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, -AXIS_X);
        break;
      case GLFW_KEY_Y:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, AXIS_X);
        break;

        // Turn the whole cube on the y-axis
      case GLFW_KEY_A:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, -AXIS_Y);
        break;
      case GLFW_KEY_SEMICOLON:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, AXIS_Y);
        break;

        // Turn the whole cube on the z-axis
      case GLFW_KEY_P:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, AXIS_Z);
        break;
      case GLFW_KEY_Q:
        for(unsigned int layer = 0; layer < cube->CUBIES_PER_EDGE; ++layer)
          cube->Twist(layer, -AXIS_Z);
        break;

        // Change the target slice
      case GLFW_KEY_LEFT_SHIFT:
        if(sliceMod == 0)
          break;
        else
          --sliceMod;
        break;
      case GLFW_KEY_RIGHT_SHIFT:
        if(sliceMod == cube->CUBIES_PER_EDGE - 1)
          break;
        else
          ++sliceMod;
        break;

        // Choose a new cube size
      case GLFW_KEY_1:
        delete cube;
        cube = new Cube(sliceMod*10+1);
        break;
      case GLFW_KEY_2:
        delete cube;
        cube = new Cube(sliceMod*10+2);
        break;
      case GLFW_KEY_3:
        delete cube;
        cube = new Cube(sliceMod*10+3);
        break;
      case GLFW_KEY_4:
        delete cube;
        cube = new Cube(sliceMod*10+4);
        break;
      case GLFW_KEY_5:
        delete cube;
        cube = new Cube(sliceMod*10+5);
        break;
      case GLFW_KEY_6:
        delete cube;
        cube = new Cube(sliceMod*10+6);
        break;
      case GLFW_KEY_7:
        delete cube;
        cube = new Cube(sliceMod*10+7);
        break;
      case GLFW_KEY_8:
        delete cube;
        cube = new Cube(sliceMod*10+8);
        break;
      case GLFW_KEY_9:
        delete cube;
        cube = new Cube(sliceMod*10+9);
        break;
      case GLFW_KEY_0:
        delete cube;
        cube = new Cube(sliceMod*10+10);
        break;

        // Default statement so compiler will stop whining about all enum cases not being handled
      default:
        break;
    }
  }
}
