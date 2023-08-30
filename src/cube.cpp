#include <GLFW/glfw3.h>
#include <random>
#include "cube.hpp"

constexpr float STICKER_WIDTH = 50.0f;
constexpr float STICKER_SPACING = STICKER_WIDTH / 20.f;

// Generate a random number between from and to
int randNum(int from, int to)
{
  static std::random_device device;
  static std::mt19937 generator(device());
  std::uniform_int_distribution distribution(from, to);

  return distribution(generator);
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

void Cube::rotateStickers(Sticker &a, Sticker &b, Sticker &c, Sticker &d, bool clockwise)
{
  a.is_rotating = true;
  b.is_rotating = true;
  c.is_rotating = true;
  d.is_rotating = true;
  Sticker tmp = a;
  if(clockwise) { a = b; b = c; c = d; d = tmp; }
  else { a = d; d = c; c = b; b = tmp; }
}

Cube::TwistInfo::TwistInfo(unsigned int layer, short axis) : axis(axis), layer(layer) {}

Cube::Cube(unsigned int cubiesPerEdge) : cubiesPerEdge(cubiesPerEdge)
{
  init();
}

Cube::~Cube()
{
  cleanup();
}

void Cube::resize(unsigned int cubiesPerEdge)
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

        if(rotateAxis == Axis::UNDEFINED)
        {
          stickers[sideNum][x][y].is_rotating = false;
        }

        // Rotate stickers if necessary
        if(stickers[sideNum][x][y].is_rotating)
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
        unsigned char stickerColor = stickers[sideNum][x][y].color;
        glColor3f(colors[stickerColor][0], colors[stickerColor][1], colors[stickerColor][2]);
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

  // Allocate all the stickers
  for(int sideNum = 0; sideNum < 6; ++sideNum)
  {
    stickers[sideNum] = new Sticker *[cubiesPerEdge];
    for(unsigned int x = 0; x < cubiesPerEdge; ++x)
    {
      stickers[sideNum][x] = new Sticker[cubiesPerEdge];
      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        stickers[sideNum][x][y].color = sideNum;
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

  int side;
  switch(axis)
  {
    case Axis::X:
      if(layer == 0)
      {
        side = Side::LEFT;
      }
      else if(layer == cubiesPerEdge-1)
      {
        side = Side::RIGHT;
      }
      else
      {
        side = -1;
      }

      for(unsigned int y = 0; y < cubiesPerEdge; y++)
      {
        rotateStickers(
            stickers[Side::FRONT][layer][y],
            stickers[Side::TOP][layer][y],
            stickers[Side::BACK][cubiesPerEdge-1-layer][cubiesPerEdge-1-y],
            stickers[Side::BOTTOM][layer][y],
            direction
        );
      }
      break;

    case Axis::Y:
      if(layer == 0)
      {
        side = Side::TOP;
      }
      else if(layer == cubiesPerEdge-1)
      {
        side = Side::BOTTOM;
      }
      else
      {
        side = -1;
      }

      for(unsigned int x = 0; x < cubiesPerEdge; x++)
      {
        rotateStickers(
          stickers[Side::LEFT][x][layer],
          stickers[Side::BACK][x][layer],
          stickers[Side::RIGHT][x][layer],
          stickers[Side::FRONT][x][layer],
          direction
        );
      }
      break;

    case Axis::Z:
      if(layer == 0)
      {
        side = Side::BACK;
      }
      else if(layer == cubiesPerEdge-1)
      {
        side = Side::FRONT;
      }
      else
      {
        side = -1;
      }

      for(unsigned int y = 0; y < cubiesPerEdge; ++y)
      {
        rotateStickers(
          stickers[Side::TOP][y][layer],
          stickers[Side::RIGHT][cubiesPerEdge-1-layer][y],
          stickers[Side::BOTTOM][cubiesPerEdge-1-y][cubiesPerEdge-1-layer],
          stickers[Side::LEFT][layer][cubiesPerEdge-1-y],
          direction
        );
      }
      break;
    default:
      side = -1;
      break;
  }

  // Here we rotate the stickers on the face of one of the sides of the cube if needed
  if(side == -1) { return; }
  if(side == Side::BACK || side == Side::LEFT || side == Side::BOTTOM)
  {
    direction = !direction;
  }

  for(unsigned int depth = 0; depth < (cubiesPerEdge+1)/2; ++depth)
  {
    if(cubiesPerEdge%2 == 1 && (cubiesPerEdge+1)/2-1 == depth)
    {
      stickers[side][depth][depth].is_rotating = true;
      break;
    }
    for(unsigned int pos = 0; pos+depth < cubiesPerEdge-1-depth + (pos+depth)%2; ++pos)
    {
      rotateStickers(
        stickers[side][depth+pos][depth],
        stickers[side][cubiesPerEdge-1-depth][depth+pos],
        stickers[side][cubiesPerEdge-1-depth-pos][cubiesPerEdge-1-depth],
        stickers[side][depth][cubiesPerEdge-1-depth-pos],
        direction
      );
    }
  }
}
