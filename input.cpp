#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "display.h"

extern cube *pCube;
extern viewPort *pCamera;
extern sf::Window *pWindow;

// display.cpp
extern void createCube(unsigned int cubiesPerEdge = 3);

// Find a way to move this into HandleInput
void viewPort::update()
{
  static sf::Clock clock;
  float elapsedTime = clock.getElapsedTime().asMilliseconds();
  clock.restart();

  //
  // Rotate the camera if needed
  //

  // Rotate on the x-axis
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    xAngle += elapsedTime / 5000.f * 360.f;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    xAngle -= elapsedTime / 5000.f * 360.f;

  // Rotate the camera on the y-axis
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    yAngle -= elapsedTime / 3000.f * 360.f;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    yAngle += elapsedTime / 3000.f * 360.f;

  // Zoom in and out
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
    zoom += elapsedTime / 3000.f * pCube->CUBIES_PER_EDGE * 100.f;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
    zoom -= elapsedTime / 3000.f * pCube->CUBIES_PER_EDGE * 100.f;

  // This part should be in display.cpp
  glTranslatef(0.f, 0.f, zoom);
  glRotatef(xAngle, 1.f, 0.f, 0.f);
  glRotatef(yAngle, 0.f, 1.f, 0.f);
}

// Get all input not related to camera control
void handleInput()
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
      viewPort::resizeWindow(event.size.width, event.size.height);

    // Handle Keyboard Input
    else if(event.type == sf::Event::KeyPressed)
    {
      switch(event.key.code)
      {
        // Quit on escape
        case sf::Keyboard::Escape:
          pWindow->close();
          break;

        // Scramble the cube
        case sf::Keyboard::Tab:
          pCube->scramble(pCube->CUBIES_PER_EDGE * 10);
          break;

          // Turn the front face
        case sf::Keyboard::H:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE - 1 - sliceMod, AXIS_Z);
          break;
        case sf::Keyboard::G:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE - 1 - sliceMod, -AXIS_Z);
          break;

          // Turn the top face
        case sf::Keyboard::J:
          pCube->twistLayer(sliceMod, AXIS_Y);
          break;
        case sf::Keyboard::F:
          pCube->twistLayer(sliceMod, -AXIS_Y);
          break;

          // Turn the left face
        case sf::Keyboard::D:
          pCube->twistLayer(sliceMod, -AXIS_X);
          break;
        case sf::Keyboard::E:
          pCube->twistLayer(sliceMod, AXIS_X);
          break;

          // Turn the right face
        case sf::Keyboard::K:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_X);
          break;
        case sf::Keyboard::I:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE-1-sliceMod, AXIS_X);
          break;

          // Turn the bottom face
        case sf::Keyboard::L:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE-1-sliceMod, AXIS_Y);
          break;
        case sf::Keyboard::S:
          pCube->twistLayer(pCube->CUBIES_PER_EDGE-1-sliceMod, -AXIS_Y);
          break;

          // Turn the back face
        case sf::Keyboard::O:
          pCube->twistLayer(sliceMod, AXIS_Z);
          break;
        case sf::Keyboard::W:
          pCube->twistLayer(sliceMod, -AXIS_Z);
          break;

        // Turn the whole cube on the x-axis
        case sf::Keyboard::B:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, -AXIS_X);
          break;
        case sf::Keyboard::Y:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, AXIS_X);
          break;

          // Turn the whole cube on the y-axis
        case sf::Keyboard::A:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, -AXIS_Y);
          break;
        case sf::Keyboard::SemiColon:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, AXIS_Y);
          break;

          // Turn the whole cube on the z-axis
        case sf::Keyboard::P:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, AXIS_Z);
          break;
        case sf::Keyboard::Q:
          for(unsigned int layer = 0; layer < pCube->CUBIES_PER_EDGE; layer++)
            pCube->twistLayer(layer, -AXIS_Z);
          break;

          // Change the target slice
        case sf::Keyboard::LShift:
          if(sliceMod == 0)
            break;
          else
            sliceMod--;
          break;
        case sf::Keyboard::RShift:
          if(sliceMod == pCube->CUBIES_PER_EDGE - 1)
            break;
          else
            sliceMod++;
          break;

          // Reset the Camera
        case sf::Keyboard::BackSpace:
          *pCamera = viewPort(pWindow->getSize().x, pWindow->getSize().y);
          break;

          // Choose a new cube size
        case sf::Keyboard::Num1:
          createCube(sliceMod*10+1);
          break;
        case sf::Keyboard::Num2:
          createCube(sliceMod*10+2);
          break;
        case sf::Keyboard::Num3:
          createCube(sliceMod*10+3);
          break;
        case sf::Keyboard::Num4:
          createCube(sliceMod*10+4);
          break;
        case sf::Keyboard::Num5:
          createCube(sliceMod*10+5);
          break;
        case sf::Keyboard::Num6:
          createCube(sliceMod*10+6);
          break;
        case sf::Keyboard::Num7:
          createCube(sliceMod*10+7);
          break;
        case sf::Keyboard::Num8:
          createCube(sliceMod*10+8);
          break;
        case sf::Keyboard::Num9:
          createCube(sliceMod*10+9);
          break;
        case sf::Keyboard::Num0:
          createCube(sliceMod*10+10);
          break;

          // Default statement so compiler will stop whining about all enum cases not being handled
        default:
          break;
      }
    }
  }
}

