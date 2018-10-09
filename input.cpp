#include "display.h"

extern cube *pCube;
extern viewPort *pCamera;

// display.cpp
extern void createCube(unsigned int cubiesPerEdge = 3);

// TODO Find a way to move this into HandleInput
void viewPort::Update()
{
  static sf::Clock clock;
  float elapsedTime = clock.getElapsedTime().asMilliseconds();
  clock.restart();

  //
  // Rotate the camera if needed
  //

  // Rotate camera on the x-axis
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Down))
    xAngle += elapsedTime / 5000.f * 360.f;
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Up))
    xAngle -= elapsedTime / 5000.f * 360.f;

  // Rotate camera on the y-axis
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Left))
    yAngle -= elapsedTime / 3000.f * 360.f;
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Right))
    yAngle += elapsedTime / 3000.f * 360.f;

  // Zoom in and out
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Equal))
    zoom += elapsedTime / 3000.f * pCube->CUBIES_PER_EDGE * 100.f;
  if(GLFW_KEY_isKeyPressed(GLFW_KEY_Dash))
    zoom -= elapsedTime / 3000.f * pCube->CUBIES_PER_EDGE * 100.f;

  // TODO This part should be in display.cpp
  glTranslatef(0.f, 0.f, zoom);
  glRotatef(xAngle, 1.f, 0.f, 0.f);
  glRotatef(yAngle, 0.f, 1.f, 0.f);
}

// Get all input not related to camera control
void HandleInput()
{
  static unsigned int sliceMod = 0;
  sf::Event event;
  while(pWindow->pollEvent(event))
  {
    // Quit
    if(event.type == sf::Event::Closed)
      pWindow->close();

    // Adjust the viewport if the window is resized
    else if(event.type == sf::Event::Resized)
      viewPort::ResizeWindow(event.size.width, event.size.height);

    // Handle Keyboard Input
    else if(event.type == sf::Event::KeyPressed)
    {
      switch(event.key.code)
      {
        // Quit on escape
        case GLFW_KEY_Escape:
          pWindow->close();
          break;

        // Scramble the cube
        case GLFW_KEY_Tab:
          pCube->Scramble(pCube->CUBIES_PER_EDGE * 10);
          break;

          // Turn the front face
        case GLFW_KEY_H:
          pCube->Twist(pCube->CUBIES_PER_EDGE - 1 - sliceMod, AXIS_Z);
          break;
        case GLFW_KEY_G:
          pCube->Twist(pCube->CUBIES_PER_EDGE - 1 - sliceMod, -AXIS_Z);
          break;

          // Turn the top face
        case GLFW_KEY_J:
          pCube->Twist(sliceMod, AXIS_Y);
          break;
        case GLFW_KEY_F:
          pCube->Twist(sliceMod, -AXIS_Y);
          break;

          // Turn the left face
        case GLFW_KEY_D:
          pCube->Twist(sliceMod, -AXIS_X);
          break;
        case GLFW_KEY_E:
          pCube->Twist(sliceMod, AXIS_X);
          break;

          // Turn the right face
        case GLFW_KEY_K:
          pCube->Twist(pCube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_X);
          break;
        case GLFW_KEY_I:
          pCube->Twist(pCube->CUBIES_PER_EDGE-1-sliceMod, AXIS_X);
          break;

          // Turn the bottom face
        case GLFW_KEY_L:
          pCube->Twist(pCube->CUBIES_PER_EDGE-1-sliceMod, AXIS_Y);
          break;
        case GLFW_KEY_S:
          pCube->Twist(pCube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_Y);
          break;

          // Turn the back face
        case GLFW_KEY_O:
          pCube->Twist(sliceMod, AXIS_Z);
          break;
        case GLFW_KEY_W:
          pCube->Twist(sliceMod, -AXIS_Z);
          break;

        // Turn the whole cube on the x-axis
        case GLFW_KEY_B:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, -AXIS_X);
          break;
        case GLFW_KEY_Y:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, AXIS_X);
          break;

          // Turn the whole cube on the y-axis
        case GLFW_KEY_A:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, -AXIS_Y);
          break;
        case GLFW_KEY_SemiColon:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, AXIS_Y);
          break;

          // Turn the whole cube on the z-axis
        case GLFW_KEY_P:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, AXIS_Z);
          break;
        case GLFW_KEY_Q:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->Twist(layer, -AXIS_Z);
          break;

          // Change the target slice
        case GLFW_KEY_LShift:
          if(sliceMod == 0)
            break;
          else
            sliceMod--;
          break;
        case GLFW_KEY_RShift:
          if(sliceMod == pCube->CUBIES_PER_EDGE - 1)
            break;
          else
            sliceMod++;
          break;

          // Reset the Camera
        case GLFW_KEY_BackSpace:
          *pCamera = viewPort(pWindow->getSize().x, pWindow->getSize().y);
          break;

          // Choose a new cube size
        case GLFW_KEY_Num1:
          createCube(sliceMod*10+1);
          break;
        case GLFW_KEY_Num2:
          createCube(sliceMod*10+2);
          break;
        case GLFW_KEY_Num3:
          createCube(sliceMod*10+3);
          break;
        case GLFW_KEY_Num4:
          createCube(sliceMod*10+4);
          break;
        case GLFW_KEY_Num5:
          createCube(sliceMod*10+5);
          break;
        case GLFW_KEY_Num6:
          createCube(sliceMod*10+6);
          break;
        case GLFW_KEY_Num7:
          createCube(sliceMod*10+7);
          break;
        case GLFW_KEY_Num8:
          createCube(sliceMod*10+8);
          break;
        case GLFW_KEY_Num9:
          createCube(sliceMod*10+9);
          break;
        case GLFW_KEY_Num0:
          createCube(sliceMod*10+10);
          break;

          // Default statement so compiler will stop whining about all enum cases not being handled
        default:
          break;
      }
    }
  }
}

