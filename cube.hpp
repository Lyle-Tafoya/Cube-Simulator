#ifndef CUBE_HPP_
#define CUBE_HPP_

#include <queue>

#define SIDE_TOP          0
#define SIDE_BOTTOM       1
#define SIDE_FRONT        2
#define SIDE_BACK         3
#define SIDE_LEFT         4
#define SIDE_RIGHT        5

#define STICKER_WIDTH     50.0f
#define STICKER_SPACING   (STICKER_WIDTH / 20.f)

#define DIR_CLOCKWISE     0
#define DIR_C_CLOCKWISE   1

#define AXIS_UNDEFINED    0
#define AXIS_X            1
#define AXIS_Y            2
#define AXIS_Z            3

// A NxNxN cube twisty puzzle
class Cube
{
public:
  // Information about a twist in the queue
  struct twistInfo
  {
    short axis;
    unsigned int layer;
    twistInfo(unsigned int init_layer, short init_axis);
  };
  std::queue<twistInfo> twist_queue;

  // A single sticker
  class sticker
  {
    unsigned char color;
    bool rotating;
  public:
    sticker(short sideNum);
    friend class Cube;
  };
private:

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
  float rotateAngle;     // The current angle of rotation (0-90)
  short rotateAxis;      // The axis we are rotating around
  sticker ***stickers[6]; // A 3D array of stickers

  void Cleanup();
  void Init();

public:
  Cube(size_t cubiesPerEdge = 3);
  ~Cube();

  void Resize(size_t cubiesPerEdge);
  void Draw(float deltaTime);
  size_t GetSize();
  void Scramble(unsigned int numTwists = 20);
  void Twist(unsigned int layer, short axis);

  friend class sticker;
};

#endif
