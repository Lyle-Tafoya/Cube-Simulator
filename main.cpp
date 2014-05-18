#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "display.h"

// input.cpp
extern void HandleInput();

// Global Variables
cube *pCube;
sf::Window *pWindow;
viewPort *pCamera;

int main()
{
  pCube = new cube();

  // Create the main window
  pWindow = new sf::Window(sf::VideoMode(800, 600, 32), "Cube Simulator", sf::Style::Default, sf::ContextSettings(32));
  pCamera = new viewPort(800, 600);

  // Main Loop
  while(pWindow->isOpen())
  {
    HandleInput();


    // Clear and draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

      pCamera->Update();
      pCube->Display();
      pWindow->display();

    glPopMatrix();
  }

  return 0;
}
