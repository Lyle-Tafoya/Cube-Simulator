#ifndef DISPLAY_H
#define DISPLAY_H

#include "cube.h"

class viewPort
{
  float xAngle;
  float yAngle;
  float zoom;

public:
  viewPort(unsigned int width, unsigned int height);
  void update();

  static void resizeWindow(unsigned int width, unsigned int height);
};

#endif
