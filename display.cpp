#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "cube.h"
#include "display.h"

extern cube *pCube;
extern sf::Window *pWindow;

void createCube(unsigned int cubiesPerEdge = 3)
{
  delete pCube;
  pCube = new cube(cubiesPerEdge);
  viewPort::ResizeWindow(pWindow->getSize().x, pWindow->getSize().y);
}

// The RGB values for the 6 colors we need
const float colors[6][3] =
{
  {1.00f, 1.00f, 1.00f}, // White
  {1.00f, 1.00f, 0.00f}, // Yellow
  {1.00f, 0.00f, 0.00f}, // Red
  {1.00f, 0.65f, 0.00f}, // Orange
  {0.00f, 1.00f, 0.00f}, // Green
  {0.00f, 0.00f, 1.00f}  // Blue
};

// Draw a rectangle
inline void drawRect(float height, float aspectRatio = 1.f)
{
  glBegin(GL_QUADS);
    glVertex3f((-height)/2.f, height * aspectRatio / 2.f,     0.f);
    glVertex3f(height/2.f,    height * aspectRatio / 2.f,     0.f);
    glVertex3f(height/2.f,    (-height * aspectRatio) / 2.f,  0.f);
    glVertex3f((-height)/2.f, (-height * aspectRatio) / 2.f,  0.f);
  glEnd();
}

// Display the cube to the screen
void cube::Display()
{
  // Update the rotation animation
  static sf::Clock clock;

  if(rotate_axis != AXIS_UNDEFINED)
  {
    rotate_angle += clock.getElapsedTime().asMilliseconds() / 200.f * 90.f;

    // If we are done rotating, set our values back to default
    if(rotate_angle >= 90.f)
    {
      rotate_axis = AXIS_UNDEFINED;
      rotate_angle = 0.f;
    }
  }
  clock.restart();

  // Loop through every cubie (piece)
  for(int sideNum = 0; sideNum < 6; sideNum++)
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
      {
        glPushMatrix();

          // Deal with the rotation of stickers
          if(rotate_axis == AXIS_UNDEFINED)
              stickers[sideNum][x][y]->rotating = false;

          // Rotate stickers if necessary
          if(stickers[sideNum][x][y]->rotating == true)
          {
            switch(rotate_axis)
            {
              // Counter-Clockwise Twists
              case -AXIS_X:
                glRotatef(rotate_angle-90.f, 1.f, 0.f, 0.f);
                break;
              case -AXIS_Y:
                glRotatef(rotate_angle-90.f, 0.f, 1.f, 0.f);
                break;
              case -AXIS_Z:
                glRotatef(rotate_angle-90.f, 0.f, 0.f, 1.f);
                break;

              // Clockwise Twists
              case AXIS_X:
                glRotatef(90.f-rotate_angle, 1.f, 0.f, 0.f);
                break;
              case AXIS_Y:
                glRotatef(90.f-rotate_angle, 0.f, 1.f, 0.f);
                break;
              case AXIS_Z:
                glRotatef(90.f-rotate_angle, 0.f, 0.f, 1.f);
                break;
            }
          }

          // Rotate to the proper side
          switch(sideNum)
          {
            case SIDE_TOP:
              glRotatef(-90.f, 1.f, 0.f, 0.f);
              break;
            case SIDE_BOTTOM:
              glRotatef(90.f, 1.f, 0.f, 0.f);
              break;
            case SIDE_FRONT: // It should already be facing the front
              break;
            case SIDE_BACK:
              glRotatef(180.f, 0.f, 1.f, 0.f);
              break;
            case SIDE_LEFT:
              glRotatef(-90.f, 0.f, 1.f, 0.f);
              break;
            case SIDE_RIGHT:
              glRotatef(90.f, 0.f, 1.f, 0.f);
              break;
          }

          // Move away from the center of the cube
          glTranslatef((CUBIES_PER_EDGE-1.f)/-2.f*(STICKER_WIDTH+STICKER_SPACING)+x*(STICKER_WIDTH+STICKER_SPACING),
                       -((CUBIES_PER_EDGE-1.f)/-2.f*(STICKER_WIDTH+STICKER_SPACING)+y*(STICKER_WIDTH+STICKER_SPACING)),
                       CUBIES_PER_EDGE/2.f*STICKER_WIDTH+(CUBIES_PER_EDGE/2.f+0.5f)*STICKER_SPACING);

          // Draw the sticker
          glColor3f(colors[stickers[sideNum][x][y]->color][0], colors[stickers[sideNum][x][y]->color][1], colors[stickers[sideNum][x][y]->color][2]);
          drawRect(STICKER_WIDTH);

        glPopMatrix();
      }

  // If we have any twists waiting to be executed, do so now
  if(rotate_axis == AXIS_UNDEFINED && !twist_queue.empty())
  {
    Twist(twist_queue.front().layer, twist_queue.front().axis);
    twist_queue.pop();
  }
}

viewPort::viewPort(unsigned int width, unsigned int height)
        : xAngle(35.f), yAngle(-35.f), zoom(0.f)
{
  // Set color and depth clear value
  glClearDepth(1.f);
  glClearColor(0.6f, 0.6f, 0.6f, 0.6f);

  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  ResizeWindow(width, height);

  glLoadIdentity();
}

// Adjust the projection matrix to a new width/height
void viewPort::ResizeWindow(unsigned int width, unsigned int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  gluPerspective(90.f, (float)width / (float)height, 1.f, 10000.f);
  gluLookAt(0.f, 0.f, pCube->CUBIES_PER_EDGE * (STICKER_WIDTH + STICKER_SPACING) * 1.5f,
            0.f, 0.f, 0.f,
            0.f, 1.f, 0.f);

  glMatrixMode(GL_MODELVIEW);
}
