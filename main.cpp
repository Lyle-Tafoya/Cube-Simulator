#include "display.h"

// input.cpp
extern void handleInput();

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "display.h"

// input.cpp
extern void handleInput();

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
    handleInput();


    // Clear and draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

      pCamera->update();
      pCube->display();
      pWindow->display();

    glPopMatrix();
  }

  return EXIT_SUCCESS;
}
