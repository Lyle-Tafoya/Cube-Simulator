#ifndef CUBE_HPP_
#define CUBE_HPP_

#include <cstddef>
#include <queue>

// A NxNxN cube twisty puzzle
class Cube
{
  enum Side
  {
    TOP = 0,
    BOTTOM,
    FRONT,
    BACK,
    LEFT,
    RIGHT
  };

  enum Direction
  {
    CLOCKWISE = 0,
    C_CLOCKWISE
  };

  struct TwistInfo
  {
    short axis;
    unsigned int layer;
    TwistInfo(unsigned int layer, short axis);
  };

  float colors[6][3] =
  {
    {1.00f, 1.00f, 1.00f}, // White
    {1.00f, 1.00f, 0.00f}, // Yellow
    {1.00f, 0.00f, 0.00f}, // Red
    {1.00f, 0.65f, 0.00f}, // Orange
    {0.00f, 1.00f, 0.00f}, // Green
    {0.00f, 0.00f, 1.00f}  // Blue
  };

  struct Sticker
  {
    unsigned char color:3;
    bool is_rotating:1 = false;
  };

  unsigned int cubiesPerEdge;
  float rotateAngle;
  short rotateAxis;
  Sticker **stickers[6];
  std::queue<TwistInfo> twistQueue;

  void cleanup();
  void init();
  static void rotateStickers(Sticker &a, Sticker &b, Sticker &c, Sticker &d, bool clockwise=true);

public:
  enum Axis
  {
    UNDEFINED = 0,
    X,
    Y,
    Z
  };

  Cube(unsigned int cubiesPerEdge = 3);
  ~Cube();

  void resize(unsigned int cubiesPerEdge);
  void draw(float deltaTime);
  inline unsigned int getSize() const { return cubiesPerEdge; }
  void scramble(unsigned int numTwists = 20);
  void twist(unsigned int layer, short axis);
};

#endif
