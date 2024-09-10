#ifndef FACES_H
#define FACES_H

const int NUM_FACES = 6;
const int NUM_SIDES = 4;

enum class Colors : char { WHITE = 'W', YELLOW = 'Y', BLUE = 'B', GREEN = 'G', RED = 'R', ORANGE = 'O' };

// Side faces ordered clockwise as left, top, right, bottom
enum class Sides : int { LEFT, TOP, RIGHT, BOTTOM };

enum class Faces : char { UP = 'U', DOWN = 'D', RIGHT = 'R', LEFT = 'L', FRONT = 'F', BACK = 'B' };

#endif  // FACES_H
