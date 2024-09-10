#include <stddef.h>

#include "Cube.h"

const Piece *Beginners_fromColors(Cube *cube, const aColors *facets)
{
    // Search whole cube
    for (int i = 0; i < CUBE_NUMPIECES; i++)
        if (Piece_matchFacets(&cube->pieces[i], facets))
            return &cube->pieces[i];
    return NULL;
}

bool Beginners_matchFaces(Cube *cube, const Piece *piece, int count, const Faces *faces, bool matchAll)
{
    // Match faces
    aColors3 faceColors;
    faceColors.count = count;
    for (int i = 0; i < count; i++)
        faceColors.colors[i] = cube->faceColors.colors[faces[i]];
    return Piece_matchFaces(piece, (const aColors *)&faceColors, count > 1 && matchAll);
}

bool Beginners_matchFacet2Face(Cube *cube, const Piece *piece, Faces facet, Faces face)
{
    return Piece_matchFacet2Face(piece, cube->faceColors.colors[facet], cube->faceColors.colors[face]);
}

void Beginners_L1L2Stage(Cube *cube, const char *workingFace, 
                            const Piece *(*findPieceCB)(Cube *, Colors, Colors),
                            Colors (*preparePieceCB)(Cube *, const Piece *, Colors),
                            void (*solvePieceCB)(Cube *, const Piece *))
{
    aColors4 sideColors = Cube_sideColors(cube, UP);
    Colors prevSideColor = sideColors.colors[NUM_SIDES - 1];
    for (int i = 0; i < NUM_SIDES; i++)
    {
        Colors sideColor = sideColors.colors[i];
        const Piece *piece = findPieceCB(cube, sideColor, prevSideColor);
        // check piece in right position; do nothing
        if (!Piece_isSolved(piece))
        {
            Faces face;
            // Rotate cube sides to bring piece to front
            while (!(face = FRONT, Beginners_matchFaces(cube, piece, 1, &face, true)) ||
                    Piece_isCorner(piece) && !(face = RIGHT, Beginners_matchFaces(cube, piece, 1, &face, true)))
                Cube_rotate(cube, "U");
            Colors frontColor = preparePieceCB(cube, piece, sideColor);

            // Rotate cube sides to bring frontColor face to front
            while (cube->faceColors.colors[FRONT] != frontColor)
                Cube_rotate(cube, "U");

            // Bring piece to front
            aColors1 front = { .count = 1, .colors = {frontColor} };
            aColors1 prev = { .count = 1, .colors = {prevSideColor} };
            while (!Piece_matchFaces(piece, (const aColors *)&front, false) ||
                    Piece_isCorner(piece) && !Piece_matchFaces(piece, (const aColors *)&prev, false))
                Cube_permute(cube, workingFace);

            solvePieceCB(cube, piece);
        }
        prevSideColor = sideColor;
    }
}

const Piece *Beginners_TopCross_findPiece(Cube *cube, Colors side , Colors prev)
{
    aColors3 facets = { .count = 2, .colors = { cube->faceColors.colors[UP], side} };
    return Beginners_fromColors(cube, (const aColors *)&facets);
}

Colors Beginners_TopCross_preparePiece(Cube *cube, const Piece *piece, Colors sideColor)
{
    // send top/middle piece to down
    Faces face;
    if (face = UP, Beginners_matchFaces(cube, piece, 1, &face, true))
        Cube_permute(cube, "F2"); // piece in top layer
    else if (!(face = DOWN, Beginners_matchFaces(cube, piece, 1, &face, true)))
    {
        // piece in middle layer
        const char *moves = (face = RIGHT, Beginners_matchFaces(cube, piece, 1, &face, true) ? "FDF'" : "F'DF");
        Cube_permute(cube, moves);
    }
    return sideColor;
}

void Beginners_TopCross_solvePiece(Cube *cube, const Piece *piece)
{
    // Bring piece to up face
    const char *moves = (Beginners_matchFacet2Face(cube, piece, UP, DOWN) ? "F2" : "DRF'R'");
    Cube_permute(cube, moves);
}

void Beginners_TopCross(Cube *cube)
{
    Beginners_L1L2Stage(cube, "D",
                        Beginners_TopCross_findPiece,
                        Beginners_TopCross_preparePiece,
                        Beginners_TopCross_solvePiece);
}

const Piece *Beginners_TopCorners_findPiece(Cube *cube, Colors side , Colors prev)
{
    aColors3 facets = { .count = 3, .colors = { cube->faceColors.colors[UP], side, prev} };
    return Beginners_fromColors(cube, (const aColors *)&facets);
}

Colors Beginners_TopCorners_preparePiece(Cube *cube, const Piece *piece, Colors sideColor)
{
    // send top piece to down
    Faces face;
    if (face = UP, Beginners_matchFaces(cube, piece, 1, &face, true))
    {
        // piece in top layer
        const char *moves = (face = RIGHT, Beginners_matchFaces(cube, piece, 1, &face, true) ? "R'D'R" : "LDL'");
        Cube_permute(cube, moves);
    }
    return sideColor;
}

void Beginners_TopCorners_solvePiece(Cube *cube, const Piece *piece)
{
    // Bring piece to up face
    if (Beginners_matchFacet2Face(cube, piece, UP, DOWN))
        Cube_permute(cube, "R'D2RDR'D'R");
    else if (Beginners_matchFacet2Face(cube, piece, UP, FRONT))
        Cube_permute(cube, "FDF'");
    else
        Cube_permute(cube, "R'D'R");
}

void Beginners_TopCorners(Cube *cube)
{
    Beginners_L1L2Stage(cube, "D",
                        Beginners_TopCorners_findPiece,
                        Beginners_TopCorners_preparePiece,
                        Beginners_TopCorners_solvePiece);
}

void Beginners_MiddleFlip(Cube *cube)
{
    // Rotate cube upside down
    Cube_rotate(cube, "R2");
}

void Beginners_swapEdge(Cube *cube, bool toRight)
{
    if (toRight)
        Cube_permute(cube, "URU'R'U'F'UF");
    else
        Cube_permute(cube, "U'L'ULUFU'F'");
}

const Piece *Beginners_MiddleEdges_findPiece(Cube *cube, Colors side , Colors prev)
{
    aColors3 facets = { .count = 2, .colors = { side, prev} };
    return Beginners_fromColors(cube, (const aColors *)&facets);
}

Colors Beginners_MiddleEdges_preparePiece(Cube *cube, const Piece *piece, Colors sideColor)
{
    // send middle piece to up
    Faces face;
    if (!(face = UP, Beginners_matchFaces(cube, piece, 1, &face, true)))
        Beginners_swapEdge(cube, (face = RIGHT, Beginners_matchFaces(cube, piece, 1, &face, true)));
    const Colors *facets = piece->facets.colors;
    return (Piece_matchFacet2Face(piece, facets[0], cube->faceColors.colors[UP]) ? facets[1] : facets[0]);
}

void Beginners_MiddleEdges_solvePiece(Cube *cube, const Piece *piece)
{
    Beginners_swapEdge(cube, Beginners_matchFacet2Face(cube, piece, RIGHT, UP));
}

void Beginners_MiddleEdges(Cube *cube)
{
    Beginners_L1L2Stage(cube, "U",
                        Beginners_MiddleEdges_findPiece,
                        Beginners_MiddleEdges_preparePiece,
                        Beginners_MiddleEdges_solvePiece);
}

const Piece *Beginners_findSemiLLCross(Cube *cube, int count, const Piece *crossPieces[])
{
    Faces back_right[] = { BACK, RIGHT };
    Faces left = LEFT;
    for (int i = 0; i < count; i++)
        if (Beginners_matchFaces(cube, crossPieces[i], 2, back_right, false))
            for (int j = 0; j < count; j++)
                if (Beginners_matchFaces(cube, crossPieces[j], 1, &left, false))
                    return crossPieces[j];
    return NULL;
}

int Beginners_findPieces(int count, const Piece *src[], const Piece *dst[], bool (*cond)(const Piece *))
{
    int dst_count = 0;
    for (int i = 0; i < count; i++)
        if (cond(src[i]))
            dst[dst_count++] = src[i];
    return dst_count;
}

int Beginners_Piece_UpColor_isSolved(int count, const Piece *src[], const Piece *dst[], Colors upColor)
{
    int dst_count = 0;
    for (int i = 0; i < count; i++)
        if (Piece_matchFacet2Face(src[i], upColor, upColor))
            dst[dst_count++] = src[i];
    return dst_count;
}

void Beginners_LLCross(Cube *cube, const Piece *LLEdges[], Colors upColor)
{
    // Repeat until upcolor cross solved.
    // Find cross pieces with solved upcolor
    const Piece *crossPieces[NUM_SIDES];
    int count;
    while ((count = Beginners_Piece_UpColor_isSolved(NUM_SIDES, LLEdges, crossPieces, upColor)) < NUM_SIDES)
    {
        if (count == 2)
        {
            // Order found pieces
            const Piece *leftPiece;
            while (!(leftPiece = Beginners_findSemiLLCross(cube, count, crossPieces)))
                Cube_rotate(cube, "U");
            // Match semi-cross to left side color
            while (!Beginners_matchFacet2Face(cube, leftPiece, LEFT, LEFT))
            {
                Cube_permute(cube, "U'");
                Cube_rotate(cube, "U");            
            }
        }
        // Try to solve 4 cross pieces to upcolor
        Cube_permute(cube, "FRUR'U'F'");
    }
}

void Beginners_LLSolvedCross(Cube *cube, const Piece *LLEdges[])
{
    // Repeat until cross is solved.
    const Piece *crossSolvedPieces[NUM_SIDES];
    int count;
    while ((count = Beginners_findPieces(NUM_SIDES, LLEdges, crossSolvedPieces, Piece_isSolved)) < NUM_SIDES)
    {
        // there always are 2 solved cross pieces just moving upface
        if (count < 2)
        {
            Cube_permute(cube, "U");
            continue;                   // Eval while-condition again
        }

        // Order solved pieces
        while (!Beginners_findSemiLLCross(cube, count, crossSolvedPieces))
            Cube_rotate(cube, "U");
        // Try to solve 4 cross pieces
        Cube_permute(cube, "URUR'URU2R'");
    }
}

void Beginners_LLCorners(Cube *cube, const Piece *LLCorners[])
{
    // while not all corners in right place(RP)
    const Faces front_right[] = { FRONT, RIGHT };
    const Piece *RPCorners[NUM_SIDES];
    int count;
    while ((count = Beginners_findPieces(NUM_SIDES, LLCorners, RPCorners, Piece_isRightPlace)) < NUM_SIDES)
    {
        if (count > 0)
            while (!Beginners_matchFaces(cube, RPCorners[0], 2, front_right, true))
                Cube_rotate(cube, "U");
        Cube_permute(cube, "URU'L'UR'U'L");
    }
}

bool Beginners_LLCornersNotSolved(const Piece *piece)
{
    return !Piece_isSolved(piece);
}

void Beginners_LLSolvedCorners(Cube *cube, const Piece *LLCorners[], Colors upColor)
{
    // Search last layer
    const Piece *LLCornersNotSolved[NUM_SIDES];
    int count = Beginners_findPieces(NUM_SIDES, LLCorners, LLCornersNotSolved, Beginners_LLCornersNotSolved);

    if (count > 0)
    {
        const Faces front_right[] = { FRONT, RIGHT };
        // Order 1rst corner not solved
        while (!Beginners_matchFaces(cube, LLCornersNotSolved[0], 2, front_right, true))
            Cube_rotate(cube, "U");

        for (int i = 0; i < count; i++)
        {
            while (!Beginners_matchFaces(cube, LLCornersNotSolved[i], 2, front_right, true))
                Cube_permute(cube, "U");
            // Solve piece
            while (!Piece_matchFacet2Face(LLCornersNotSolved[i], upColor, upColor))
                Cube_permute(cube, "R'D'RD");
        }
        // Permute LL to match sides
        while (!Piece_isSolved(LLCornersNotSolved[0]))
            Cube_permute(cube, "U");
    }
}

void Beginners_solve(Cube *cube)
{
    // Top layer
    Beginners_TopCross(cube);
    Beginners_TopCorners(cube);

    Beginners_MiddleFlip(cube);

    // Middle layer
    Beginners_MiddleEdges(cube);

    // Last layer
    Colors upColor = cube->faceColors.colors[UP];

    // Search last layer edges
    const Piece *LLEdges[NUM_SIDES];
    Beginners_findPieces(FACE_NUMPIECES, (const Piece **)cube->facePieces[upColor], LLEdges, Piece_isEdge);
    Beginners_LLCross(cube, LLEdges, upColor);
    Beginners_LLSolvedCross(cube, LLEdges);

    // Search last layer corners
    const Piece *LLCorners[NUM_SIDES];
    Beginners_findPieces(FACE_NUMPIECES, (const Piece **)cube->facePieces[upColor], LLCorners, Piece_isCorner);

    Beginners_LLCorners(cube, LLCorners);
    Beginners_LLSolvedCorners(cube, LLCorners, upColor);

    Beginners_MiddleFlip(cube);
}
