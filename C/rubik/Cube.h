#include "Piece.h"
#include "Move.h"

#define CUBE_NUMPIECES 20
#define FACE_NUMPIECES 8
#define CUBE_DIM 3

typedef struct tag_Cube Cube;
typedef void (*ShowCB)(Cube *, Move *);
struct tag_Cube
{
    // Callback to print moves
    ShowCB fnShow;
 
    // Position-face to color-face map
    aColors6 faceColors;
    // Pieces of cube
    // Cube array of pieces: 8 corners + 12 edges
    Piece pieces[CUBE_NUMPIECES];
    // color-face to pieces map
    // 8 pieces per faces (excluding centerpiece)
    Piece *facePieces[NUM_FACES][FACE_NUMPIECES];
};

const Faces *Cube_sides(Faces face);
aColors4 Cube_sideColors(Cube *cube, Faces face);

void Cube_build(Cube *cube, ShowCB fnShow);

void Cube_draw(Cube *cube, Faces face, Colors layout[CUBE_DIM][CUBE_DIM]);

void Cube_rotate(Cube *cube, const char *strMoves);

void Cube_permute(Cube *cube, const char *strMoves);