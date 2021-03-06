#ifndef CUBE_HPP_
#define CUBE_HPP_

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


  size_t cubiesPerEdge;
  float rotateAngle;
  short rotateAxis;
  unsigned char **stickers[6];
  std::queue<TwistInfo> twistQueue;

  void cleanup();
  void init();
  static constexpr unsigned char STICKER_ROTATE_BIT = 128;
  static void rotateStickers(unsigned char &a, unsigned char &b, unsigned char &c, unsigned char &d, bool clockwise=true);

public:
  enum Axis
  {
    UNDEFINED = 0,
    X,
    Y,
    Z
  };

  Cube(size_t cubiesPerEdge = 3);
  ~Cube();

  void resize(size_t cubiesPerEdge);
  void draw(float deltaTime);
  inline size_t getSize() const { return cubiesPerEdge; }
  void scramble(unsigned int numTwists = 20);
  void twist(unsigned int layer, short axis);

  friend class Sticker;
};

#endif
