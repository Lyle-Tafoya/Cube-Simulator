#include <chrono>
#include "cube.hpp"
#include "inputhandler.hpp"
#include "viewport.hpp"

int main()
{
  Viewport *viewport = new Viewport(800, 600);
  Cube *cube = new Cube();
  InputHandler::Init(cube, viewport);

  float deltaTime = 0;
  std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point currentTime;
  while(!glfwWindowShouldClose(viewport->GetWindow()))
  {
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime-lastUpdate).count();
    lastUpdate = currentTime;

    glfwPollEvents();
    InputHandler::Update(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube->Draw(deltaTime);

    glfwSwapBuffers(viewport->GetWindow());
  }

  return 0;
}
