#include <stdbool.h>

// UP, DOWN, RIGHT, LEFT, FRONT, BACK
#define RUBIK_MOVES "UDRLFB"

// Default step is 1 CW.
// Extra steps: 2 CW, 1 CCW
#define RUBIK_STEPS "2'"

typedef void *HRBK;

HRBK Rubik_create(const char *Algorithm, bool showMoves);
void Rubik_destroy(HRBK hRubik);

void Rubik_print(HRBK hRubik);

// moves = "RANDOM" for random rotation
void Rubik_rotate(HRBK hRubik, const char *moves);

//  moves = "RANDOM" for random shuffle
void Rubik_shuffle(HRBK hRubik, const char *moves);

void Rubik_solve(HRBK hRubik);

bool Rubik_isSolved(HRBK hRubik);

