#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Cube.h"

// Topology invariants
//

// Initial faces color as ordered in enum declaration of Colors
// WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
// UP, DOWN, RIGHT, LEFT, FRONT, BACK
static aColors6 initFaceColors = { .count = NUM_FACES, .colors = { WHITE, YELLOW, BLUE, GREEN, RED, ORANGE }};
// Opposite of UP, DOWN, RIGHT, LEFT, FRONT, BACK
static Faces oppositeFace[NUM_FACES] = { DOWN, UP, LEFT, RIGHT, BACK, FRONT };
// Sides of UP, DOWN, RIGHT, LEFT, FRONT, BACK. Clockwise from face
static Faces sideFaces[NUM_FACES][NUM_SIDES] = 
{
    {LEFT, BACK, RIGHT, FRONT},
    {LEFT, FRONT, RIGHT, BACK},
    {FRONT, UP, BACK, DOWN},
    {BACK, UP, FRONT, DOWN},
    {LEFT, UP, RIGHT, DOWN},
    {RIGHT, UP, LEFT, DOWN}
};

const Faces *Cube_sides(Faces face)
{
    return sideFaces[face];
}

aColors4 Cube_sideColors(Cube *cube, Faces face)
{
    aColors4 sideColors;
    sideColors.count = NUM_SIDES;

    for (int i = 0; i < sideColors.count; i++)
        sideColors.colors[i] = cube->faceColors.colors[sideFaces[face][i]];

    return sideColors;
}

void Cube_buildPiece(Cube *cube, int *p, int fp[NUM_FACES], const aColors *facets)
{
    cube->pieces[*p] = Piece_create(facets);
    for (int i = 0; i < facets->count; i++)
    {
        Colors sideColor = facets->colors[i];
        cube->facePieces[sideColor][fp[sideColor]++] = &cube->pieces[*p];
    }
    (*p)++;
}

void Cube_build(Cube *cube, ShowCB fnShow)
{
    cube->fnShow = fnShow;

    // cube->faceColors will change with rotations
    cube->faceColors = initFaceColors;
 
    Colors Up = cube->faceColors.colors[UP];
    Colors Down = cube->faceColors.colors[oppositeFace[UP]];
    aColors3 UpDown = { .count = 2, .colors = { Up, Down } };
    aColors4 sideColors = Cube_sideColors(cube, UP); 

   // pieces indx
    int p = 0;
    // facePieces indx
    int fp[NUM_FACES] = { 0 };
    for (int side = 0; side < sideColors.count; side++)
    {
        int sideNext = (side + 1) % sideColors.count;

        // Middle edges
        aColors3 facets2m = { .count = 2, .colors = { sideColors.colors[side], sideColors.colors[sideNext] } };
        Cube_buildPiece(cube, &p, fp, (const aColors *)&facets2m);
        for (int sideUpDown = 0; sideUpDown < UpDown.count; sideUpDown++)
        {
            // Up, down edges
            aColors3 facets2 = { .count = 2, .colors = { sideColors.colors[side], UpDown.colors[sideUpDown] } };
            Cube_buildPiece(cube, &p, fp, (const aColors *)&facets2);
            // Corners
            aColors3 facets3 = { .count = 3, .colors = { sideColors.colors[side], sideColors.colors[sideNext], UpDown.colors[sideUpDown] } };
            Cube_buildPiece(cube, &p, fp, (const aColors *)&facets3);
        }
    }
}        

void Cube_draw(Cube *cube, Faces face, Colors layout[CUBE_DIM][CUBE_DIM])
{
    Colors faceColor = cube->faceColors.colors[face];
    aColors4 sideColors = Cube_sideColors(cube, face); 
    // Init layout with centerpiece
    layout[1][1] = faceColor;

    for (int i = 0; i < FACE_NUMPIECES; i++)
    {
        const aColors *faces = (const aColors *)&cube->facePieces[faceColor][i]->faces;
        const aColors *facets = (const aColors *)&cube->facePieces[faceColor][i]->facets;
        int row = 1, col = 1;
        Colors pieceColor = faceColor;

        for (int j = 0; j < faces->count; j++)
        {
            if (faces->colors[j] == faceColor)
                pieceColor = facets->colors[j];
            else if (faces->colors[j] == sideColors.colors[SIDES_TOP])
                row = 0;
            else if (faces->colors[j] == sideColors.colors[SIDES_BOTTOM])
                row = 2;
            if (faces->colors[j] == sideColors.colors[SIDES_LEFT])
                col = 0;
            else if (faces->colors[j] == sideColors.colors[SIDES_RIGHT])
                col = 2;
        }
        layout[row][col] = pieceColor;
    }
}

void Cube_rotate(Cube *cube, const char *strMoves)
{
    // max moves = 1 per char
    Move moves[strlen(strMoves)];

    int numMoves = Move_aparse(strMoves, moves);
    for (int m = 0; m < numMoves; m++)
    {
        Faces *sides = sideFaces[moves[m].face];
        // Copy previous face colors
        aColors6 prevFaceColors = cube->faceColors;
        for (int i = 0; i < NUM_SIDES; i++)
        {
            // Assign previous face color to current face
            int prevFace = (NUM_SIDES + i - moves[m].step) % NUM_SIDES;
            Colors prevColor = prevFaceColors.colors[sides[prevFace]];
            cube->faceColors.colors[sides[i]] = prevColor;
        }
        if (cube->fnShow)
            cube->fnShow(cube, &moves[m]);
    }
}

void Cube_permute(Cube *cube, const char *strMoves)
{
    // max moves = 1 per char
    Move moves[strlen(strMoves)];

    int numMoves = Move_aparse(strMoves, moves);
    for (int m = 0; m < numMoves; m++)
    {
        Colors faceColor = cube->faceColors.colors[moves[m].face];
        const aColors1 afaceColor = { .count = 1, .colors = { faceColor } };
        aColors4 sideColors = Cube_sideColors(cube, moves[m].face); 
        // Re-indx pieces of side faces
        int fp[NUM_SIDES] = { 0 };
        for (int i = 0; i < NUM_SIDES; i++)
        {
            Colors sideColor = sideColors.colors[i];
            for (int j = 0; j < FACE_NUMPIECES; j++)
            {
                Piece *piece = cube->facePieces[sideColor][j];
                if (!Piece_matchFaces(piece, (const aColors *)&afaceColor, false))
                    cube->facePieces[sideColor][fp[i]++] = piece;
            }
        }
        // Rotate face pieces
        for (int j = 0; j < FACE_NUMPIECES; j++)
        {
            Piece *piece = cube->facePieces[faceColor][j];
            int k = -1;
            //Only changes facet of sides
            for (int f = 0; f < piece->faces.count; f++)
                if ((k = aColors_Index((const aColors *)&sideColors, piece->faces.colors[f])) >= 0)
                {
                    int nextSide = (NUM_SIDES + k + moves[m].step) % NUM_SIDES;
                    piece->faces.colors[f] = sideColors.colors[nextSide];
                    // Add to new side
                    cube->facePieces[sideColors.colors[nextSide]][fp[nextSide]++] = piece;
                }
        }
        if (cube->fnShow)
            cube->fnShow(cube, &moves[m]);
    }
}

