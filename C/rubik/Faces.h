#ifndef FACES_H
#define FACES_H

#include <stdbool.h>

#define NUM_FACES 6
#define NUM_SIDES 4
#define MAX_FACETS 3

typedef enum tag_Colors { WHITE, YELLOW, BLUE, GREEN, RED, ORANGE } Colors;

// Side faces ordered clockwise as left, top, right, bottom
typedef enum tag_Sides { SIDES_LEFT, SIDES_TOP, SIDES_RIGHT, SIDES_BOTTOM } Sides;

typedef enum tag_Faces { UP, DOWN, RIGHT, LEFT, FRONT, BACK } Faces;

// Generic no-mem sized aColors
typedef struct tag_aColors
{
    int count;
    Colors colors[];
} aColors;

int aColors_mask(const aColors *ac);

// return -1 if not found
int aColors_Index(const aColors *ac, Colors c);
// Equal arrays until count
bool aColors_Equals(const aColors *ac1, const aColors *ac2);
// Unordered but same colors in both arrays
bool aColors_EqualSet(const aColors *ac1, const aColors *ac2);
// Test a subset of colors is all/any found in a set
bool aColors_Contains(const aColors *set, const aColors *subset, bool matchAll);

// Family of mem-sized aColors.
// Define sizes needed. Avoid dynamic mem.
//

// Faces sized aColors
typedef struct tag_aColors6
{
    int count;
    Colors colors[NUM_FACES];
} aColors6;

// Sides sized aColors
typedef struct tag_aColors4
{
    int count;
    Colors colors[NUM_SIDES];
} aColors4;

// Facets sized aColors
typedef struct tag_aColors3
{
    int count;
    Colors colors[MAX_FACETS];
} aColors3;

// 1 color sized aColors
typedef struct tag_aColors1
{
    int count;
    Colors colors[1];
} aColors1;

#endif  // FACES_H
