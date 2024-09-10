#include "Faces.h"

typedef struct tag_Piece
{
    // Pieces have 2 or 3 facets/faces
    // Pieces are invariant in colors. Facets are invariant after creation
    aColors3 facets;
    // Pieces faces are referenced to centerpieces colors(Invariant for rotations)
    aColors3 faces;
} Piece;

// Create ordered: facets and faces are the same color.
Piece Piece_create(const aColors *facets);

// Piece assertions
bool Piece_isEdge(const Piece *piece);
bool Piece_isCorner(const Piece *piece);
bool Piece_isRightPlace(const Piece *piece);
bool Piece_isSolved(const Piece *piece);
bool Piece_matchFacets(const Piece *piece, const aColors *facets);
bool Piece_matchFaces(const Piece *piece, const aColors *faces, bool matchAll);

// Facet assertion
bool Piece_matchFacet2Face(const Piece *piece, Colors facet, Colors face);

