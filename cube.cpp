#include <cstdlib>
#include <ctime>

#include "cube.h"

// Generate a random number between from and to
int randNum(int from, int to)
{
  return (rand() % (to - from + 1)) + from;
}

cube::twistInfo::twistInfo(unsigned int initLayer, short initAxis)
               : layer(initLayer), axis(initAxis) {}

cube::sticker::sticker(short side_num)
             : color(side_num), rotating(false) {}

cube::cube(unsigned int cubiesPerEdge)
    : rotate_axis(AXIS_UNDEFINED), rotate_angle(0.f), CUBIES_PER_EDGE(cubiesPerEdge)
{
  // Seed the random number generator
  srand(time(NULL));

  // Create/initialize all the stickers
  for(int sideNum = 0; sideNum < 6; sideNum++)
  {
    stickers[sideNum] = new sticker **[CUBIES_PER_EDGE];
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
    {
      stickers[sideNum][x] = new sticker *[CUBIES_PER_EDGE];
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
        stickers[sideNum][x][y] = new sticker(sideNum);
    }
  }
}

// Delete the memory we allocated for the stickers
cube::~cube()
{
  for(int sideNum = 0; sideNum < 6; sideNum++)
  {
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
    {
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
        delete stickers[sideNum][x][y];
      delete[] stickers[sideNum][x];
    }
    delete[] stickers[sideNum];
  }
}

// Scramble the cube
void cube::Scramble(unsigned int numTwists)
{
  for(unsigned int i = 0; i < numTwists; i++)
    Twist(randNum(0, CUBIES_PER_EDGE-1), randNum(AXIS_X, AXIS_Z));
}

// Twist a single layer of the rubik's cube along a given axis
void cube::Twist(unsigned int layer, short axis)
{
  // We can't twist a layer that doesn't exist
  if(layer >= CUBIES_PER_EDGE)
    return;

  // If we are in the middle of rotating, add this twist to the queue
  if(rotate_axis != AXIS_UNDEFINED && (axis != rotate_axis || !twist_queue.empty()))
  {
    twist_queue.push(twistInfo(layer, axis));
    return;
  }

  // TODO Replace this with a single temporary sticker to save memory
  // Create a temporary array of stickers to work with
  sticker ***tmp[6];
  for(int sideNum = 0; sideNum < 6; sideNum++)
  {
    tmp[sideNum] = new sticker **[CUBIES_PER_EDGE];
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
    {
      tmp[sideNum][x] = new sticker *[CUBIES_PER_EDGE];
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
        // Initialize it to our existing sticker array
        tmp[sideNum][x][y] = stickers[sideNum][x][y];
    }
  }

  // Let the cube know that it's supposed to start rotating
  rotate_axis = axis;

  // Determine if we are rotating clockwise or counter clockwise
  bool direction = DIR_CLOCKWISE;
  if(axis < 0)
  {
    direction = DIR_C_CLOCKWISE;
    axis = -axis;
  }

  int side;
  switch(axis)
  {
    // Rotate a layer around x-axis
    case AXIS_X:
      side = ((layer == 0) ? SIDE_LEFT : ((layer == (CUBIES_PER_EDGE - 1)) ? SIDE_RIGHT : -1));

      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
      {
        if(direction == DIR_CLOCKWISE)
        {
          tmp[SIDE_FRONT][layer][y] = stickers[SIDE_BOTTOM][layer][y];
          tmp[SIDE_BOTTOM][layer][y] = stickers[SIDE_BACK][CUBIES_PER_EDGE-1-layer][CUBIES_PER_EDGE-1-y];
          tmp[SIDE_BACK][CUBIES_PER_EDGE-1-layer][y] = stickers[SIDE_TOP][layer][CUBIES_PER_EDGE-1-y];
          tmp[SIDE_TOP][layer][y] = stickers[SIDE_FRONT][layer][y];
        }
        else
        {
          tmp[SIDE_FRONT][layer][y] = stickers[SIDE_TOP][layer][y];
          tmp[SIDE_TOP][layer][y] = stickers[SIDE_BACK][CUBIES_PER_EDGE-1-layer][CUBIES_PER_EDGE-1-y];
          tmp[SIDE_BACK][CUBIES_PER_EDGE-1-layer][y] = stickers[SIDE_BOTTOM][layer][CUBIES_PER_EDGE-1-y];
          tmp[SIDE_BOTTOM][layer][y] = stickers[SIDE_FRONT][layer][y];
        }

        // Start rotating the stickers
        tmp[SIDE_FRONT][layer][y]->rotating = true;
        tmp[SIDE_TOP][layer][y]->rotating = true;
        tmp[SIDE_BOTTOM][layer][y]->rotating = true;
        tmp[SIDE_BACK][CUBIES_PER_EDGE-1-layer][y]->rotating = true;
      }
      break;

    // Rotate a layer around the y-axis
    case AXIS_Y:
      side = ((layer == 0) ? SIDE_TOP : ((layer == (CUBIES_PER_EDGE - 1)) ? SIDE_BOTTOM : -1));

      for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
      {
        if(direction == DIR_CLOCKWISE)
        {
          tmp[SIDE_FRONT][x][layer] = stickers[SIDE_RIGHT][x][layer];
          tmp[SIDE_RIGHT][x][layer] = stickers[SIDE_BACK][x][layer];
          tmp[SIDE_BACK][x][layer] = stickers[SIDE_LEFT][x][layer];
          tmp[SIDE_LEFT][x][layer] = stickers[SIDE_FRONT][x][layer];

        }
        else
        {
          tmp[SIDE_RIGHT][x][layer] = stickers[SIDE_FRONT][x][layer];
          tmp[SIDE_BACK][x][layer] = stickers[SIDE_RIGHT][x][layer];
          tmp[SIDE_LEFT][x][layer] = stickers[SIDE_BACK][x][layer];
          tmp[SIDE_FRONT][x][layer] = stickers[SIDE_LEFT][x][layer];
        }

        // Start rotating the stickers
        tmp[SIDE_FRONT][x][layer]->rotating = true;
        tmp[SIDE_RIGHT][x][layer]->rotating = true;
        tmp[SIDE_BACK][x][layer]->rotating = true;
        tmp[SIDE_LEFT][x][layer]->rotating = true;
      }
      break;

    // Rotate a layer around the z-axis
    case AXIS_Z:
      side = ((layer == 0) ? SIDE_BACK : ((layer == (CUBIES_PER_EDGE - 1)) ? SIDE_FRONT : -1));

      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
      {
        if(direction == DIR_CLOCKWISE)
        {
          tmp[SIDE_LEFT][layer][y] = stickers[SIDE_BOTTOM][y][CUBIES_PER_EDGE-1-layer];
          tmp[SIDE_BOTTOM][y][CUBIES_PER_EDGE-1-layer] = stickers[SIDE_RIGHT][CUBIES_PER_EDGE-1-layer][CUBIES_PER_EDGE-1-y];
          tmp[SIDE_RIGHT][CUBIES_PER_EDGE-1-layer][y] = stickers[SIDE_TOP][y][layer];
          tmp[SIDE_TOP][y][layer] = stickers[SIDE_LEFT][layer][CUBIES_PER_EDGE-1-y];
        }
        else
        {
          tmp[SIDE_LEFT][layer][y] = stickers[SIDE_TOP][CUBIES_PER_EDGE-1-y][layer];
          tmp[SIDE_TOP][y][layer] = stickers[SIDE_RIGHT][CUBIES_PER_EDGE-1-layer][y];
          tmp[SIDE_RIGHT][CUBIES_PER_EDGE-1-layer][y] = stickers[SIDE_BOTTOM][CUBIES_PER_EDGE-1-y][CUBIES_PER_EDGE-1-layer];
          tmp[SIDE_BOTTOM][y][CUBIES_PER_EDGE-1-layer] = stickers[SIDE_LEFT][layer][y];
        }

        // Start rotating the stickers
        tmp[SIDE_LEFT][layer][y]->rotating = true;
        tmp[SIDE_TOP][y][layer]->rotating = true;
        tmp[SIDE_RIGHT][CUBIES_PER_EDGE-1-layer][y]->rotating = true;
        tmp[SIDE_BOTTOM][y][CUBIES_PER_EDGE-1-layer]->rotating = true;
      }
      break;
  }

  // Here we rotate the stickers on the face of one of the sides of the cube if needed
  if(side != -1)
  {
    if(side == SIDE_BACK || side == SIDE_LEFT || side == SIDE_BOTTOM)
      direction = !direction;
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
      {
        if(direction == DIR_CLOCKWISE)
          tmp[side][x][y] = stickers[side][y][CUBIES_PER_EDGE-1-x];
        else
          tmp[side][x][y] = stickers[side][CUBIES_PER_EDGE-1-y][x];
        tmp[side][x][y]->rotating = true;
      }
  }

  // Copy everything from the tmp array and delete allocated memory
  for(int sideNum = 0; sideNum < 6; sideNum++)
  {
    for(unsigned int x = 0; x < CUBIES_PER_EDGE; x++)
    {
      for(unsigned int y = 0; y < CUBIES_PER_EDGE; y++)
        stickers[sideNum][x][y] = tmp[sideNum][x][y];
      delete[] tmp[sideNum][x];
    }
    delete[] tmp[sideNum];
  }
}
