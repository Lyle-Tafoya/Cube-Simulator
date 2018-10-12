#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "cube.hpp"

#define STICKER_WIDTH     50.0f
#define STICKER_SPACING   (STICKER_WIDTH / 20.f)

// Generate a random number between from and to
int randNum(int from, int to)
{
  return (rand() % (to - from + 1)) + from;
}

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

Cube::TwistInfo::TwistInfo(unsigned int layer, short axis) : axis(axis), layer(layer) {}
Cube::Sticker::Sticker(unsigned char side) : color(side), rotating(false) {}

Cube::Cube(size_t cubiesPerEdge) : cubiesPerEdge(cubiesPerEdge)
{
  init();
  srand(time(NULL));
}

Cube::~Cube()
{
  cleanup();
}

void Cube::resize(size_t cubiesPerEdge)
{
  cleanup();
  this->cubiesPerEdge = cubiesPerEdge;
  init();
}

void Cube::cleanup()
{
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        delete stickers[sideNum][x][y];
      }
      delete[] stickers[sideNum][x];
    }
    delete[] stickers[sideNum];
  }
}

void Cube::draw(float deltaTime)
{
  if(rotateAxis != Axis::UNDEFINED)
  {
    rotateAngle += deltaTime*1000.f / 200.f * 90.f;
    if(rotateAngle >= 90.f)
    {
      rotateAxis = Axis::UNDEFINED;
      rotateAngle = 0.f;
    }
  }

  // Loop through every sticker
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        glPushMatrix();

        // Deal with the rotation of stickers
        if(rotateAxis == Axis::UNDEFINED)
        {
          stickers[sideNum][x][y]->rotating = false;
        }

        // Rotate stickers if necessary
        if(stickers[sideNum][x][y]->rotating == true)
        {
          switch(rotateAxis)
          {
            // Counter-Clockwise Twists
            case -Axis::X:
              glRotatef(rotateAngle-90.f, 1.f, 0.f, 0.f);
              break;
            case -Axis::Y:
              glRotatef(rotateAngle-90.f, 0.f, 1.f, 0.f);
              break;
            case -Axis::Z:
              glRotatef(rotateAngle-90.f, 0.f, 0.f, 1.f);
              break;

              // Clockwise Twists
            case Axis::X:
              glRotatef(90.f-rotateAngle, 1.f, 0.f, 0.f);
              break;
            case Axis::Y:
              glRotatef(90.f-rotateAngle, 0.f, 1.f, 0.f);
              break;
            case Axis::Z:
              glRotatef(90.f-rotateAngle, 0.f, 0.f, 1.f);
              break;
          }
        }

        // Rotate to the proper side
        switch(sideNum)
        {
          case Side::TOP:
            glRotatef(-90.f, 1.f, 0.f, 0.f);
            break;
          case Side::BOTTOM:
            glRotatef(90.f, 1.f, 0.f, 0.f);
            break;
          case Side::FRONT: // It should already be facing the front
            break;
          case Side::BACK:
            glRotatef(180.f, 0.f, 1.f, 0.f);
            break;
          case Side::LEFT:
            glRotatef(-90.f, 0.f, 1.f, 0.f);
            break;
          case Side::RIGHT:
            glRotatef(90.f, 0.f, 1.f, 0.f);
            break;
        }

        // Move away from the center of the cube
        glTranslatef((cubiesPerEdge-1.f)/-2.f*(STICKER_WIDTH+STICKER_SPACING)+x*(STICKER_WIDTH+STICKER_SPACING),
            -((cubiesPerEdge-1.f)/-2.f*(STICKER_WIDTH+STICKER_SPACING)+y*(STICKER_WIDTH+STICKER_SPACING)),
            cubiesPerEdge/2.f*STICKER_WIDTH+(cubiesPerEdge/2.f+0.5f)*STICKER_SPACING);

        // Draw the sticker
        glColor3f(colors[stickers[sideNum][x][y]->color][0], colors[stickers[sideNum][x][y]->color][1], colors[stickers[sideNum][x][y]->color][2]);
        drawRect(STICKER_WIDTH);

        glPopMatrix();
      }
    }
  }

  // If we have any twists waiting to be executed, do so now
  if(rotateAxis == Axis::UNDEFINED && !twistQueue.empty())
  {
    twist(twistQueue.front().layer, twistQueue.front().axis);
    twistQueue.pop();
  }
}

void Cube::init()
{
  rotateAngle = 0.f;
  rotateAxis = Axis::UNDEFINED;

  // Create/initialize all the stickers
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    stickers[sideNum] = new Sticker **[cubiesPerEdge];
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      stickers[sideNum][x] = new Sticker *[cubiesPerEdge];
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        stickers[sideNum][x][y] = new Sticker(sideNum);
      }
    }
  }
}

// Scramble the Cube
void Cube::scramble(unsigned int numTwists)
{
  for(unsigned int i = 0; i < numTwists; ++i)
  {
    twist(randNum(0, cubiesPerEdge-1), randNum(Axis::X, Axis::Z));
  }
}

// Twist a single layer of the rubik's Cube along a given axis
void Cube::twist(unsigned int layer, short axis)
{
  if(layer >= cubiesPerEdge) { return; }

  // If we are in the middle of rotating, add this twist to the queue
  if(rotateAxis != Axis::UNDEFINED && (axis != rotateAxis || !twistQueue.empty()))
  {
    twistQueue.push(TwistInfo(layer, axis));
    return;
  }

  rotateAxis = axis;

  // Determine if we are rotating clockwise or counter clockwise
  bool direction = Direction::CLOCKWISE;
  if(axis < 0)
  {
    direction = Direction::C_CLOCKWISE;
    axis = -axis;
  }

  Sticker *tmp;
  int side;
  switch(axis)
  {
    case Axis::X:
      side = ((layer == 0) ? Side::LEFT : ((layer == (cubiesPerEdge - 1)) ? Side::RIGHT : -1));

      for(size_t y = 0; y < cubiesPerEdge; y++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp = stickers[Side::FRONT][layer][y];
          stickers[Side::FRONT][layer][y] = stickers[Side::BOTTOM][layer][y];
          stickers[Side::BOTTOM][layer][y] = stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y] = stickers[Side::TOP][layer][y];
          stickers[Side::TOP][layer][y] = tmp;
        }
        else
        {
          tmp = stickers[Side::TOP][layer][y];
          stickers[Side::TOP][layer][y] = stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y] = stickers[Side::BOTTOM][layer][y];
          stickers[Side::BOTTOM][layer][y] = stickers[Side::FRONT][layer][y];
          stickers[Side::FRONT][layer][y] = tmp;
        }

        stickers[Side::FRONT][layer][y]->rotating = true;
        stickers[Side::BOTTOM][layer][y]->rotating = true;
        stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y]->rotating = true;
        stickers[Side::TOP][layer][y]->rotating = true;
      }
      break;

    case Axis::Y:
      side = ((layer == 0) ? Side::TOP : ((layer == (cubiesPerEdge - 1)) ? Side::BOTTOM : -1));

      for(size_t x = 0; x < cubiesPerEdge; x++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp = stickers[Side::FRONT][x][layer];
          stickers[Side::FRONT][x][layer] = stickers[Side::RIGHT][x][layer];
          stickers[Side::RIGHT][x][layer] = stickers[Side::BACK][x][layer];
          stickers[Side::BACK][x][layer] = stickers[Side::LEFT][x][layer];
          stickers[Side::LEFT][x][layer] = tmp;

        }
        else
        {
          tmp = stickers[Side::LEFT][x][layer];
          stickers[Side::LEFT][x][layer] = stickers[Side::BACK][x][layer];
          stickers[Side::BACK][x][layer] = stickers[Side::RIGHT][x][layer];
          stickers[Side::RIGHT][x][layer] = stickers[Side::FRONT][x][layer];
          stickers[Side::FRONT][x][layer] = tmp;
        }

        stickers[Side::FRONT][x][layer]->rotating = true;
        stickers[Side::RIGHT][x][layer]->rotating = true;
        stickers[Side::BACK][x][layer]->rotating = true;
        stickers[Side::LEFT][x][layer]->rotating = true;
      }
      break;

    case Axis::Z:
      side = ((layer == 0) ? Side::BACK : ((layer == (cubiesPerEdge - 1)) ? Side::FRONT : -1));

      for(size_t y = 0; y < cubiesPerEdge; ++y)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp = stickers[Side::LEFT][layer][cubiesPerEdge-1-y];
          stickers[Side::LEFT][layer][cubiesPerEdge-1-y] = stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer];
          stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer] = stickers[Side::RIGHT][cubiesPerEdge-1-layer][y];
          stickers[Side::RIGHT][cubiesPerEdge-1-layer][y] = stickers[Side::TOP][y][layer];
          stickers[Side::TOP][y][layer] = tmp;
        }
        else
        {
          tmp = stickers[Side::TOP][y][layer];
          stickers[Side::TOP][y][layer] = stickers[Side::RIGHT][cubiesPerEdge-1-layer][y];
          stickers[Side::RIGHT][cubiesPerEdge-1-layer][y] = stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer];
          stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer] = stickers[Side::LEFT][layer][cubiesPerEdge-1-y];
          stickers[Side::LEFT][layer][cubiesPerEdge-1-y] = tmp;
        }

        stickers[Side::LEFT][layer][cubiesPerEdge-1-y]->rotating = true;
        stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer]->rotating = true;
        stickers[Side::RIGHT][cubiesPerEdge-1-layer][y]->rotating = true;
        stickers[Side::TOP][y][layer]->rotating = true;
      }
      break;
  }

  // Here we rotate the stickers on the face of one of the sides of the cube if needed
  if(side == -1) { return; }
  if(side == Side::BACK || side == Side::LEFT || side == Side::BOTTOM)
  {
    direction = !direction;
  }

  // This algorithm spirals in towards the center, operating on 4 stickers at a time with a single temp
  unsigned char dir = 1;
  size_t numIterations = static_cast<int>(ceil(cubiesPerEdge*cubiesPerEdge/4.l));
  for(size_t x = 0, y = 0, xLimit = cubiesPerEdge-2-x, yLimit = cubiesPerEdge-2-y, i = 0; i < numIterations; ++i)
  {
    tmp = stickers[side][x][y];
    for(size_t i = 0, xPos = x, yPos = y; i < 3; ++i)
    {
      if(direction == Direction::CLOCKWISE)
      {
        stickers[side][xPos][yPos] = stickers[side][yPos][cubiesPerEdge-1-xPos];
        size_t tmpPos = xPos;
        xPos = yPos;
        yPos = cubiesPerEdge-1-tmpPos;
      }
      else
      {
        stickers[side][xPos][yPos] = stickers[side][cubiesPerEdge-1-yPos][xPos];
        size_t tmpPos = xPos;
        xPos = cubiesPerEdge-1-yPos;
        yPos = tmpPos;
      }
      stickers[side][xPos][yPos]->rotating = true;
    }
    tmp->rotating = true;
    if(direction == Direction::CLOCKWISE)
    {
      stickers[side][cubiesPerEdge-1-y][x] = tmp;
    }
    else
    {
      stickers[side][y][cubiesPerEdge-1-x] = tmp;
    }
    switch(dir % 4)
    {
      case 0:
        if(--y == yLimit)
        {
          ++dir;
          xLimit = cubiesPerEdge-2-x;
        }
        break;
      case 1:
        if(++x == xLimit)
        {
          ++dir;
          yLimit = cubiesPerEdge-2-y;
        }
        break;
      case 2:
        if(++y == yLimit)
        {
          ++dir;
          xLimit = cubiesPerEdge-1-x;
        }
        break;
      case 3:
        if(--x == yLimit)
        {
          ++dir;
          yLimit = cubiesPerEdge-1-y;
        }
        break;
    }
  }
}
