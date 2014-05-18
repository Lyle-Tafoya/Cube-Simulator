#ifndef CUBE_H
#define CUBE_H

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
class cube
{
  // Information about a twist in the queue
  struct twistInfo
  {
    unsigned int layer;
    short axis;
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

    friend class cube;
  };

  sticker ***stickers[6]; // A 3D array of stickers
  short rotate_axis;      // The axis we are rotating around
  float rotate_angle;     // The current angle of rotation (0-90)

public:
  cube(unsigned int cubies_per_edge = 3);
  ~cube();

  void Display();
  void Twist(unsigned int layer, short axis);
  void Scramble(unsigned int num_twists = 20);

  const unsigned int CUBIES_PER_EDGE;

  friend class sticker;
};

#endif
