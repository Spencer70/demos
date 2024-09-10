#include <string.h>
#include <stdlib.h>

#include "Rubik.h"
#include "Move.h"

Move Move_parse(const char *strMove)
{
    Move move;

    char *p = RUBIK_MOVES;
    move.face = (Faces)(strchr(p, strMove[0]) - p);

    p = RUBIK_STEPS;
    char *s = strchr(p, strMove[1]);
    move.step = (s && *s ? 2 + (s - p) : 1);

    return move;
}

int Move_aparse(const char *strMoves, Move moves[])
{
    int count = 0;
    for (const char *strMove = strMoves; *strMove; strMove++, count++)
    {
        moves[count] = Move_parse(strMove);
        if (moves[count].step > 1)
            strMove++;
    }
    return count;
}

void Move_print(Move move, char strMove[])
{
    int i = 0;
    strMove[i] = RUBIK_MOVES[move.face];
    move.step -=2;
    if (move.step >= 0)
        strMove[++i] = RUBIK_STEPS[move.step];
    strMove[++i] = 0;
}

Move Move_random()
{
    Move move;
    
    move.face = rand() % (sizeof RUBIK_MOVES - 1);  // 0..5
    move.step = 1 + rand() % (sizeof RUBIK_STEPS);  // 1..3

    return move;
}

void Move_arandom(int numMoves, char strMoves[])
{
    for (char *strMove = strMoves; --numMoves >= 0; strMove++)
    {
        Move move = Move_random();
        Move_print(move, strMove);
        if (move.step > 1)
            strMove++;
    }
}
