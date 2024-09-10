#include "Cube3.hpp"
#include "Beginners.hpp"
#include <algorithm>
#include <functional>

static Rubik_Factory::Algorithm_Factory<Cube3_Beginners> beginners_factory("Beginners");

Cube3_Beginners::Cube3_Beginners(bool showMoves) : Cube3(showMoves) {}

void Cube3_Beginners::solve()
{
    // Top layer
    TopCross();
   
    TopCorners();
    MiddleFlip();

    // Middle layer
    MiddleEdges();

    // Last layer
    Colors upColor = faceColors[Faces::UP];

    // Search last layer edges
    auto LLEdges = findPieces(facePieces[upColor], [](Piece *p){ return p->isEdge(); });

    LLCross(LLEdges, upColor);
    LLSolvedCross(LLEdges);

    // Search last layer corners
    auto LLCorners = findPieces(facePieces[upColor], [](Piece *p){ return p->isCorner(); });

    LLRightPosCorners(LLCorners);
    LLSolvedCorners(LLCorners, upColor);

    MiddleFlip();

}

const Piece *Cube3_Beginners::fromColors(const std::vector<Colors> &facets)
{
    // Search whole cube
    for (Piece &piece : pieces)
        if (piece.matchFacets(facets))
            return &piece;
    return nullptr;
}

bool Cube3_Beginners::matchFaces(const Piece *piece, const std::vector<Faces> &faces, bool matchAll)
{
    // Match faces
    std::vector<Colors> faceColors;
    for (auto &face : faces)
        faceColors.push_back(this->faceColors[face]);

    return piece->matchFaces(faceColors, faceColors.size() > 1 && matchAll);
}

bool Cube3_Beginners::matchFacet2Face(const Piece *piece, Faces facet, Faces face)
{
    return piece->matchFacet2Face(faceColors[facet], faceColors[face]);
}

void Cube3_Beginners::L1L2Stage(const char *workingFace,
                const Piece *(Cube3_Beginners::*findPieceCB)(Colors, Colors),
                Colors (Cube3_Beginners::*preparePieceCB)(const Piece *, Colors),
                void (Cube3_Beginners::*solvePieceCB)(const Piece *))
{
    auto sideColors = this->sideColors(Faces::UP);
    Colors prevSideColor = sideColors[NUM_SIDES - 1];
    for (Colors sideColor : sideColors)
    {
        const Piece *piece = (this->*findPieceCB)(sideColor, prevSideColor);
        // check piece in right position; do nothing
        if (!piece->isSolved())
        {
            // Rotate cube sides to bring piece to front
            while (!matchFaces(piece, { Faces::FRONT }) ||
                    piece->isCorner() && !matchFaces(piece, { Faces::RIGHT }))
                _rotate("U");
            Colors frontColor = (this->*preparePieceCB)(piece, sideColor);

            // Rotate cube sides to bring frontColor face to front
            while (faceColors[Faces::FRONT] != frontColor)
                _rotate("U");

            // Bring piece to front
            while (!piece->matchFaces({ frontColor }) ||
                    piece->isCorner() && !piece->matchFaces({ prevSideColor }))
                permute(workingFace);

            (this->*solvePieceCB)(piece);
        }
        prevSideColor = sideColor;
    }
}

const Piece *Cube3_Beginners::TopCross_findPiece(Colors side, Colors prev)
{
    return fromColors({ faceColors[Faces::UP], side });
}

Colors Cube3_Beginners::TopCross_preparePiece(const Piece *piece, Colors sideColor)
{
    // send top/middle piece to down
    if (matchFaces(piece, { Faces::UP }))
        permute("F2"); // piece in top layer
    else if (!matchFaces(piece, { Faces::DOWN }))
    {
        // piece in middle layer
        const char *moves = (matchFaces(piece, { Faces::RIGHT }) ? "FDF'" : "F'DF");
        permute(moves);
    }
    return sideColor;
}

void Cube3_Beginners::TopCross_solvePiece(const Piece *piece)
{
    // Bring piece to up face
    const char *moves = (matchFacet2Face(piece, Faces::UP, Faces::DOWN) ? "F2" : "DRF'R'");
    permute(moves);
}

void Cube3_Beginners::TopCross()
{
    L1L2Stage("D",
                &Cube3_Beginners::TopCross_findPiece,
                &Cube3_Beginners::TopCross_preparePiece,
                &Cube3_Beginners::TopCross_solvePiece);
}

const Piece *Cube3_Beginners::TopCorners_findPiece(Colors side , Colors prev)
{
    return fromColors({ faceColors[Faces::UP], side, prev });
}

Colors Cube3_Beginners::TopCorners_preparePiece(const Piece *piece, Colors sideColor)
{
    // send top piece to down
    if (matchFaces(piece, { Faces::UP }))
    {
        // piece in top layer
        const char *moves = (matchFaces(piece, { Faces::RIGHT }) ? "R'D'R" : "LDL'");
        permute(moves);
    }
    return sideColor;
}

void Cube3_Beginners::TopCorners_solvePiece(const Piece *piece)
{
    // Bring piece to up face
    const char *moves;
    if (matchFacet2Face(piece, Faces::UP, Faces::DOWN))
        moves = "R'D2RDR'D'R";
    else if (matchFacet2Face(piece, Faces::UP, Faces::FRONT))
        moves = "FDF'";
    else
        moves = "R'D'R";

    permute(moves);
}

void Cube3_Beginners::TopCorners()
{
    L1L2Stage("D",
                &Cube3_Beginners::TopCorners_findPiece,
                &Cube3_Beginners::TopCorners_preparePiece,
                &Cube3_Beginners::TopCorners_solvePiece);
}

void Cube3_Beginners::MiddleFlip()
{
    // Rotate cube upside down
    _rotate("R2");
}

void Cube3_Beginners::swapEdge(bool toRight)
{
    const char *moves = (toRight ? "URU'R'U'F'UF" : "U'L'ULUFU'F'");
    permute(moves);
}

const Piece *Cube3_Beginners::MiddleEdges_findPiece(Colors side , Colors prev)
{
    return fromColors({ side, prev });
}

Colors Cube3_Beginners::MiddleEdges_preparePiece(const Piece *piece, Colors sideColor)
{
    // send middle piece to up
    if (!matchFaces(piece, { Faces::UP }))
        swapEdge(matchFaces(piece, { Faces::RIGHT }));

    std::vector<Colors> facets = { piece->cbegin()->first, piece->crbegin()->first };
    return (piece->matchFacet2Face(facets.front(), faceColors[Faces::UP]) ? facets.back() : facets.front());
}

void Cube3_Beginners::MiddleEdges_solvePiece(const Piece *piece)
{
    swapEdge(matchFacet2Face(piece, Faces::RIGHT, Faces::UP));
}

void Cube3_Beginners::MiddleEdges()
{
    L1L2Stage("U",
                &Cube3_Beginners::MiddleEdges_findPiece,
                &Cube3_Beginners::MiddleEdges_preparePiece,
                &Cube3_Beginners::MiddleEdges_solvePiece);
}


// F is callable type with 1 param
template<typename F>
std::vector<Piece *> Cube3_Beginners::findPieces(std::vector<Piece *> src, F f)
{
    std::vector<Piece *> dst;

    std::copy_if(src.begin(), src.end(), std::back_inserter(dst), f);
    
    return dst;
}

const Piece *Cube3_Beginners::findSemiLLCross(std::vector<Piece *> crossPieces)
{
    for (Piece *piece1 : crossPieces)
        if (matchFaces(piece1, { Faces::BACK, Faces::RIGHT }, false))
            for (Piece *piece2 : crossPieces)
                if (matchFaces(piece2, { Faces::LEFT }))
                    return piece2;
    return nullptr;
}

void Cube3_Beginners::LLCross(std::vector<Piece *> LLEdges, const Colors upColor)
{
    // Repeat until upcolor cross solved.
    // Find cross pieces with solved upcolor
    auto condition = [upColor](Piece *p){ return p->matchFacet2Face(upColor, upColor);};
    std::vector<Piece *> crossPieces;
    while ((crossPieces = findPieces(LLEdges, condition)).size() < NUM_SIDES)    
    {
        if (crossPieces.size() == 2)
        {
            // Order found pieces
            const Piece *leftPiece;
            while (!(leftPiece = findSemiLLCross(crossPieces)))
                _rotate("U");
            // Match semi-cross to left side color
            while (!matchFacet2Face(leftPiece, Faces::LEFT, Faces::LEFT))
            {
                permute("U'");
                _rotate("U");            
            }
        }
        // Try to solve 4 cross pieces to upcolor
        permute("FRUR'U'F'");
    }
}

void Cube3_Beginners::LLSolvedCross(std::vector<Piece *> LLEdges)
{
    // Repeat until cross is solved.
    auto condition = [](Piece *p){ return p->isSolved(); };
    std::vector<Piece *> crossSolvedPieces;
    while ((crossSolvedPieces = findPieces(LLEdges, condition)).size() < NUM_SIDES)
    {
        // there always are 2 solved cross pieces just moving upface
        if (crossSolvedPieces.size() < 2)
        {
            permute("U");
            continue;                   // Eval while-condition again
        }

        // Order solved pieces
        while (!findSemiLLCross(crossSolvedPieces))
            _rotate("U");
        // Try to solve 4 cross pieces
        permute("URUR'URU2R'");
    }
}

void Cube3_Beginners::LLRightPosCorners(std::vector<Piece *> LLCorners)
{
    // while not all corners in right place(RP)
    auto condition = [](Piece *p){ return p->isRightPlace(); };
    std::vector<Piece *> RPCorners;
    while ((RPCorners = findPieces(LLCorners, condition)).size() < NUM_SIDES)
    {
        if (RPCorners.size() > 0)
            while (!matchFaces(RPCorners.front(), { Faces::FRONT, Faces::RIGHT }))
                _rotate("U");
        permute("URU'L'UR'U'L");
    }
}

void Cube3_Beginners::LLSolvedCorners(std::vector<Piece *> LLCorners, Colors upColor)
{
    // Search last layer
    std::vector<Piece *> LLCornersNotSolved = findPieces(LLCorners, [](Piece *p){ return !p->isSolved(); });

    if (LLCornersNotSolved.size() > 0)
    {
        const std::vector<Faces> front_right = { Faces::FRONT, Faces::RIGHT };
        // Order 1rst corner not solved
        while (!matchFaces(LLCornersNotSolved.front(), { Faces::FRONT, Faces::RIGHT }))
            _rotate("U");

        for (Piece *piece : LLCornersNotSolved)
        {
            while (!matchFaces(piece, { Faces::FRONT, Faces::RIGHT }))
                permute("U");
            // Solve piece
            while (!piece->matchFacet2Face(upColor, upColor))
                permute("R'D'RD");
        }
         // Permute LL to match sides
        while (!LLCornersNotSolved.front()->isSolved())
            permute("U");
    }
}

