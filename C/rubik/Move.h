#include "Faces.h"

typedef struct tag_Move
{
    Faces face;
    int step;
} Move;

// Print string. strMove size >= 3
void Move_print(Move move, char strMove[]);

Move Move_parse(const char *strMove);
// return number of parsed moves
int Move_aparse(const char *strMoves, Move moves[]);

Move Move_random();
// return number of random created moves
void Move_arandom(int numMoves, char strMoves[]);
