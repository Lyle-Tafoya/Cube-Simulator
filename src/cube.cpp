#include <GLFW/glfw3.h>
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
        delete tmpStickers[sideNum][x][y];
      }
      delete[] stickers[sideNum][x];
      delete[] tmpStickers[sideNum][x];
    }
    delete[] stickers[sideNum];
    delete[] tmpStickers[sideNum];
  }
}

void Cube::draw(float deltaTime)
{
  // Update the rotation animation
  if(rotateAxis != Axis::UNDEFINED)
  {
    rotateAngle += deltaTime*1000.f / 200.f * 90.f;

    // If we are done rotating, set our values back to default
    if(rotateAngle >= 90.f)
    {
      rotateAxis = Axis::UNDEFINED;
      rotateAngle = 0.f;
    }
  }

  // Loop through every cubie (piece)
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
    tmpStickers[sideNum] = new Sticker **[cubiesPerEdge];
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      stickers[sideNum][x] = new Sticker *[cubiesPerEdge];
      tmpStickers[sideNum][x] = new Sticker *[cubiesPerEdge];
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        stickers[sideNum][x][y] = new Sticker(sideNum);
        tmpStickers[sideNum][x][y] = new Sticker(sideNum);
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
  // We can't twist a layer that doesn't exist
  if(layer >= cubiesPerEdge) { return; }

  // If we are in the middle of rotating, add this twist to the queue
  if(rotateAxis != Axis::UNDEFINED && (axis != rotateAxis || !twistQueue.empty()))
  {
    twistQueue.push(TwistInfo(layer, axis));
    return;
  }

  for(int side = 0; side < 6; ++side)
  {
    for(size_t x = 0; x < cubiesPerEdge; ++x)
    {
      for(size_t y = 0; y < cubiesPerEdge; ++y)
      {
        *tmpStickers[side][x][y] = *stickers[side][x][y];
      }
    }
  }

  // Let the Cube know that it's supposed to start rotating
  rotateAxis = axis;

  // Determine if we are rotating clockwise or counter clockwise
  bool direction = Direction::CLOCKWISE;
  if(axis < 0)
  {
    direction = Direction::C_CLOCKWISE;
    axis = -axis;
  }

  int side;
  switch(axis)
  {
    // Rotate a layer around x-axis
    case Axis::X:
      side = ((layer == 0) ? Side::LEFT : ((layer == (cubiesPerEdge - 1)) ? Side::RIGHT : -1));

      for(size_t y = 0; y < cubiesPerEdge; y++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          *stickers[Side::FRONT][layer][y] = *tmpStickers[Side::BOTTOM][layer][y];
          *stickers[Side::BOTTOM][layer][y] = *tmpStickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          *stickers[Side::BACK][cubiesPerEdge-1-layer][y] = *tmpStickers[Side::TOP][layer][cubiesPerEdge-1-y];
          *stickers[Side::TOP][layer][y] = *tmpStickers[Side::FRONT][layer][y];
        }
        else
        {
          *stickers[Side::FRONT][layer][y] = *tmpStickers[Side::TOP][layer][y];
          *stickers[Side::TOP][layer][y] = *tmpStickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          *stickers[Side::BACK][cubiesPerEdge-1-layer][y] = *tmpStickers[Side::BOTTOM][layer][cubiesPerEdge-1-y];
          *stickers[Side::BOTTOM][layer][y] = *tmpStickers[Side::FRONT][layer][y];
        }

        // Start rotating the stickers
        stickers[Side::FRONT][layer][y]->rotating = true;
        stickers[Side::TOP][layer][y]->rotating = true;
        stickers[Side::BOTTOM][layer][y]->rotating = true;
        stickers[Side::BACK][cubiesPerEdge-1-layer][y]->rotating = true;
      }
      break;

      // Rotate a layer around the y-axis
    case Axis::Y:
      side = ((layer == 0) ? Side::TOP : ((layer == (cubiesPerEdge - 1)) ? Side::BOTTOM : -1));

      for(size_t x = 0; x < cubiesPerEdge; x++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          *stickers[Side::FRONT][x][layer] = *tmpStickers[Side::RIGHT][x][layer];
          *stickers[Side::RIGHT][x][layer] = *tmpStickers[Side::BACK][x][layer];
          *stickers[Side::BACK][x][layer] = *tmpStickers[Side::LEFT][x][layer];
          *stickers[Side::LEFT][x][layer] = *tmpStickers[Side::FRONT][x][layer];

        }
        else
        {
          *stickers[Side::RIGHT][x][layer] = *tmpStickers[Side::FRONT][x][layer];
          *stickers[Side::BACK][x][layer] = *tmpStickers[Side::RIGHT][x][layer];
          *stickers[Side::LEFT][x][layer] = *tmpStickers[Side::BACK][x][layer];
          *stickers[Side::FRONT][x][layer] = *tmpStickers[Side::LEFT][x][layer];
        }

        // Start rotating the stickers
        stickers[Side::FRONT][x][layer]->rotating = true;
        stickers[Side::RIGHT][x][layer]->rotating = true;
        stickers[Side::BACK][x][layer]->rotating = true;
        stickers[Side::LEFT][x][layer]->rotating = true;
      }
      break;

      // Rotate a layer around the z-axis
    case Axis::Z:
      side = ((layer == 0) ? Side::BACK : ((layer == (cubiesPerEdge - 1)) ? Side::FRONT : -1));

      for(size_t y = 0; y < cubiesPerEdge; ++y)
      {
        if(direction == Direction::CLOCKWISE)
        {
          *stickers[Side::LEFT][layer][y] = *tmpStickers[Side::BOTTOM][y][cubiesPerEdge-1-layer];
          *stickers[Side::BOTTOM][y][cubiesPerEdge-1-layer] = *tmpStickers[Side::RIGHT][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          *stickers[Side::RIGHT][cubiesPerEdge-1-layer][y] = *tmpStickers[Side::TOP][y][layer];
          *stickers[Side::TOP][y][layer] = *tmpStickers[Side::LEFT][layer][cubiesPerEdge-1-y];
        }
        else
        {
          *stickers[Side::LEFT][layer][y] = *tmpStickers[Side::TOP][cubiesPerEdge-1-y][layer];
          *stickers[Side::TOP][y][layer] = *tmpStickers[Side::RIGHT][cubiesPerEdge-1-layer][y];
          *stickers[Side::RIGHT][cubiesPerEdge-1-layer][y] = *tmpStickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer];
          *stickers[Side::BOTTOM][y][cubiesPerEdge-1-layer] = *tmpStickers[Side::LEFT][layer][y];
        }

        // Start rotating the stickers
        stickers[Side::LEFT][layer][y]->rotating = true;
        stickers[Side::TOP][y][layer]->rotating = true;
        stickers[Side::RIGHT][cubiesPerEdge-1-layer][y]->rotating = true;
        stickers[Side::BOTTOM][y][cubiesPerEdge-1-layer]->rotating = true;
      }
      break;
  }

  // Here we rotate the stickers on the face of one of the sides of the cube if needed
  if(side != -1)
  {
    if(side == Side::BACK || side == Side::LEFT || side == Side::BOTTOM)
    {
      direction = !direction;
    }
    for(size_t x = 0; x < cubiesPerEdge; ++x)
    {
      for(size_t y = 0; y < cubiesPerEdge; ++y)
      {
        if(direction == Direction::CLOCKWISE)
        {
          *stickers[side][x][y] = *tmpStickers[side][y][cubiesPerEdge-1-x];
        }
        else
        {
          *stickers[side][x][y] = *tmpStickers[side][cubiesPerEdge-1-y][x];
        }
        stickers[side][x][y]->rotating = true;
      }
    }
  }
}
