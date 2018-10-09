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
      }
      delete[] stickers[sideNum][x];
    }
    delete[] stickers[sideNum];
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
  // We can't twist a layer that doesn't exist
  if(layer >= cubiesPerEdge) { return; }

  // If we are in the middle of rotating, add this twist to the queue
  if(rotateAxis != Axis::UNDEFINED && (axis != rotateAxis || !twistQueue.empty()))
  {
    twistQueue.push(TwistInfo(layer, axis));
    return;
  }

  // Create a temporary array of stickers to work with
  Sticker ***tmp[6];
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    tmp[sideNum] = new Sticker **[cubiesPerEdge];
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      tmp[sideNum][x] = new Sticker *[cubiesPerEdge];
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        tmp[sideNum][x][y] = stickers[sideNum][x][y];
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

      for(unsigned int y = 0; y < cubiesPerEdge; y++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp[Side::FRONT][layer][y] = stickers[Side::BOTTOM][layer][y];
          tmp[Side::BOTTOM][layer][y] = stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          tmp[Side::BACK][cubiesPerEdge-1-layer][y] = stickers[Side::TOP][layer][cubiesPerEdge-1-y];
          tmp[Side::TOP][layer][y] = stickers[Side::FRONT][layer][y];
        }
        else
        {
          tmp[Side::FRONT][layer][y] = stickers[Side::TOP][layer][y];
          tmp[Side::TOP][layer][y] = stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          tmp[Side::BACK][cubiesPerEdge-1-layer][y] = stickers[Side::BOTTOM][layer][cubiesPerEdge-1-y];
          tmp[Side::BOTTOM][layer][y] = stickers[Side::FRONT][layer][y];
        }

        // Start rotating the stickers
        tmp[Side::FRONT][layer][y]->rotating = true;
        tmp[Side::TOP][layer][y]->rotating = true;
        tmp[Side::BOTTOM][layer][y]->rotating = true;
        tmp[Side::BACK][cubiesPerEdge-1-layer][y]->rotating = true;
      }
      break;

      // Rotate a layer around the y-axis
    case Axis::Y:
      side = ((layer == 0) ? Side::TOP : ((layer == (cubiesPerEdge - 1)) ? Side::BOTTOM : -1));

      for(unsigned int x = 0; x < cubiesPerEdge; x++)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp[Side::FRONT][x][layer] = stickers[Side::RIGHT][x][layer];
          tmp[Side::RIGHT][x][layer] = stickers[Side::BACK][x][layer];
          tmp[Side::BACK][x][layer] = stickers[Side::LEFT][x][layer];
          tmp[Side::LEFT][x][layer] = stickers[Side::FRONT][x][layer];

        }
        else
        {
          tmp[Side::RIGHT][x][layer] = stickers[Side::FRONT][x][layer];
          tmp[Side::BACK][x][layer] = stickers[Side::RIGHT][x][layer];
          tmp[Side::LEFT][x][layer] = stickers[Side::BACK][x][layer];
          tmp[Side::FRONT][x][layer] = stickers[Side::LEFT][x][layer];
        }

        // Start rotating the stickers
        tmp[Side::FRONT][x][layer]->rotating = true;
        tmp[Side::RIGHT][x][layer]->rotating = true;
        tmp[Side::BACK][x][layer]->rotating = true;
        tmp[Side::LEFT][x][layer]->rotating = true;
      }
      break;

      // Rotate a layer around the z-axis
    case Axis::Z:
      side = ((layer == 0) ? Side::BACK : ((layer == (cubiesPerEdge - 1)) ? Side::FRONT : -1));

      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp[Side::LEFT][layer][y] = stickers[Side::BOTTOM][y][cubiesPerEdge-1-layer];
          tmp[Side::BOTTOM][y][cubiesPerEdge-1-layer] = stickers[Side::RIGHT][cubiesPerEdge-1-layer][cubiesPerEdge-1-y];
          tmp[Side::RIGHT][cubiesPerEdge-1-layer][y] = stickers[Side::TOP][y][layer];
          tmp[Side::TOP][y][layer] = stickers[Side::LEFT][layer][cubiesPerEdge-1-y];
        }
        else
        {
          tmp[Side::LEFT][layer][y] = stickers[Side::TOP][cubiesPerEdge-1-y][layer];
          tmp[Side::TOP][y][layer] = stickers[Side::RIGHT][cubiesPerEdge-1-layer][y];
          tmp[Side::RIGHT][cubiesPerEdge-1-layer][y] = stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer];
          tmp[Side::BOTTOM][y][cubiesPerEdge-1-layer] = stickers[Side::LEFT][layer][y];
        }

        // Start rotating the stickers
        tmp[Side::LEFT][layer][y]->rotating = true;
        tmp[Side::TOP][y][layer]->rotating = true;
        tmp[Side::RIGHT][cubiesPerEdge-1-layer][y]->rotating = true;
        tmp[Side::BOTTOM][y][cubiesPerEdge-1-layer]->rotating = true;
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
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        if(direction == Direction::CLOCKWISE)
        {
          tmp[side][x][y] = stickers[side][y][cubiesPerEdge-1-x];
        }
        else
        {
          tmp[side][x][y] = stickers[side][cubiesPerEdge-1-y][x];
        }
        tmp[side][x][y]->rotating = true;
      }
    }
  }

  // Copy everything from the tmp array and delete allocated memory
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        stickers[sideNum][x][y] = tmp[sideNum][x][y];
      }
      delete[] tmp[sideNum][x];
    }
    delete[] tmp[sideNum];
  }
}
