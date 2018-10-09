#include <chrono>
#include "cube.hpp"
#include "inputhandler.hpp"
#include "viewport.hpp"

int main()
{
  Viewport *viewport = new Viewport(800, 600);
  GLFWwindow *window = viewport->getWindow();
  Cube *cube = new Cube();
  InputHandler::init(cube, viewport);

  float deltaTime = 0;
  std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point currentTime;
  while(!glfwWindowShouldClose(window))
  {
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime-lastUpdate).count();
    lastUpdate = currentTime;

    glfwPollEvents();
    InputHandler::update(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube->draw(deltaTime);
    glfwSwapBuffers(window);
  }

  return 0;
}
