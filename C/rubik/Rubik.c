#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "Rubik.h"
#include "Cube.h"

#define NUM_ROTATIONS 5
#define NUM_SHUFFLE 10
#define MAX_MOVES 100
#define MAX_NAME 100

typedef void (*Solver)(Cube *);
typedef struct tag_rubikAlgorithm { const char *name; Solver solve; } rAlg;
typedef struct tag_Rubik
{
    // Algorithm selector
    Solver fnAlgorithm;
    // Save moves for test algorithms
    char strMoves[MAX_MOVES + 1];
    char strRotations[MAX_MOVES + 1];

    // Cube
    Cube cube;
} Rubik;

// Mapping algorithm solvers
//

#define DECLARE_SOLVER(name) void name##_solve(Cube *cube);
#define ON_SOLVER(name) { #name, name##_solve },
#define BEGIN_SOLVER_MAP() static rAlg rAlgs[] = {
#define END_SOLVER_MAP() { NULL, NULL } }; \
    Solver Rubik_getSolver(const char *name) \
    { rAlg *a; for (a = rAlgs; a->name && strcmp(a->name, name); a++); return a->solve; }

// Add DECLARE/ON_SOLVER for desired algorithms to solve cubes
DECLARE_SOLVER(Test)
DECLARE_SOLVER(Beginners)

BEGIN_SOLVER_MAP()
    ON_SOLVER(Test)
    ON_SOLVER(Beginners)
END_SOLVER_MAP()

HRBK Rubik_create(const char *Algorithm, bool showMoves)
{
    // Look up algorithm by name
    Solver solver = Rubik_getSolver(Algorithm ? Algorithm : "Test");
    if (!solver)
        return NULL;

    Rubik *rubik = (Rubik *)malloc(sizeof(Rubik));

    rubik->fnAlgorithm = solver;
    rubik->strMoves[0] = 0;
    rubik->strRotations[0] = 0;
    srand((uintptr_t)rubik);

    // Setup show callback
    void Rubik_show(Cube *cube, Move *move);
    ShowCB fnShow = (showMoves ? Rubik_show : NULL);

   // build cube
    Cube_build(&rubik->cube, fnShow);

    return (HRBK)rubik;
}
    
void Rubik_destroy(HRBK hRubik) { free(hRubik); }

// Colored letters to print facets
// WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
static const char *colorCodes[NUM_FACES] =
{
    // color_escape      letter  reset_escape
    "\e[1;38;5;015m"     "W"     "\e[0m",
    "\e[1;38;5;226m"     "Y"     "\e[0m",
    "\e[1;38;5;012m"     "B"     "\e[0m",
    "\e[1;38;5;010m"     "G"     "\e[0m",
    "\e[1;38;5;009m"     "R"     "\e[0m",
    "\e[1;38;5;166m"     "O"     "\e[0m"
};

void Rubik_printRow(int leftWidth, Colors row[], const char *end)
{
    printf("%*s %s %s %s", leftWidth, colorCodes[row[0]], colorCodes[row[1]], colorCodes[row[2]], end);
}

void Rubik_show(Cube *cube, Move *move)
{
    // Optional. Print move
    if (move)
    {
        // move + step + null
        char strMove[3];
        Move_print(*move, strMove);
        printf("%s\n", strMove);
    }

    // print cube faces unfolded
    Colors layout[CUBE_DIM][CUBE_DIM];
    int leftWidth = 2 * CUBE_DIM * 4; // 4 = 2 * escape + letter + white-space

    // UP
    Cube_draw(cube, UP, layout);
    for (int i = 0; i < CUBE_DIM; i++)
        Rubik_printRow(leftWidth, layout[i], "\n");

    // LEFT - FRONT - RIGHT - BACK. Sides of DOWN
    Colors layout4[NUM_SIDES][CUBE_DIM][CUBE_DIM];
    const Faces *sideFaces = Cube_sides(DOWN);
    for (int i = 0; i < NUM_SIDES; i++)
        Cube_draw(cube, sideFaces[i], layout4[i]);
    for (int i = 0; i < CUBE_DIM; i++)
    {
        for (int j = 0; j < NUM_SIDES; j++)
            Rubik_printRow(0, layout4[j][i], "");
        printf("\n");
    }

    // DOWN
    Cube_draw(cube, DOWN, layout);
    for (int i = 0; i < CUBE_DIM; i++)
        Rubik_printRow(leftWidth, layout[i], "\n");
}

void Rubik_print(HRBK hRubik)
{
    Rubik *rubik = (Rubik *)hRubik;

    Rubik_show(&rubik->cube, NULL);
}

// moves = "RANDOM" for random rotation
void Rubik_rotate(HRBK hRubik, const char *moves)
{
    Rubik *rubik = (Rubik *)hRubik;

    if (strcmp(moves, "RANDOM") == 0)
        Move_arandom(NUM_ROTATIONS, rubik->strRotations);
    else
        strncpy(rubik->strRotations, moves, MAX_MOVES);
    
    printf("Rotation: %s\n", rubik->strRotations);

    Cube_rotate(&rubik->cube, rubik->strRotations);
}

// moves = "RANDOM" for random shuffle
void Rubik_shuffle(HRBK hRubik, const char *moves)
{
    Rubik *rubik = (Rubik *)hRubik;

    if (strcmp(moves, "RANDOM") == 0)
        Move_arandom(NUM_SHUFFLE + rand() % NUM_SHUFFLE, rubik->strMoves);
    else
        strncpy(rubik->strMoves, moves, MAX_MOVES);

    printf("Shuffle: %s\n", rubik->strMoves);

    Cube_permute(&rubik->cube, rubik->strMoves);
}

bool Rubik_isSolved(HRBK hRubik)
{
    Rubik *rubik = (Rubik *)hRubik;

    for (int p = 0; p < CUBE_NUMPIECES; p++)
        if (!Piece_isSolved(&rubik->cube.pieces[p]))
            return false;
    return true;
}

void Rubik_solve(HRBK hRubik)
{
    Rubik *rubik = (Rubik *)hRubik;

    rubik->fnAlgorithm(&rubik->cube);
}

void Test_solve(Cube *cube)
{
    // Test algorithm.
    // Take stored moves from rubik struct and reverse them.
    Rubik *rubik = (Rubik *)((char *)cube - offsetof(Rubik, cube));
    const char *strMoves = rubik->strMoves;
    Move moves[strlen(strMoves)];

    int count = Move_aparse(strMoves, moves);
    char strReversedMoves[2 * count + 1];
    for (char *strMove = strReversedMoves; --count >= 0; strMove++)
    {
        if (moves[count].step != 2)
            moves[count].step = (moves[count].step + 2) % 4;
        Move_print(moves[count], strMove);
        if (moves[count].step > 1)
            strMove++;
    }
    Cube_permute(cube, strReversedMoves);
}