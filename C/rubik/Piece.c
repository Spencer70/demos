
#include "Piece.h"

// Create ordered: facets and faces are the same color.
Piece Piece_create(const aColors *facets)
{
    Piece piece;
    piece.facets.count = piece.faces.count = facets->count;
    for (int i = 0; i < facets->count; i++)
        piece.facets.colors[i] = piece.faces.colors[i] = facets->colors[i];
    return piece;
}

bool Piece_isEdge(const Piece *piece) { return (piece->facets.count == 2); }
bool Piece_isCorner(const Piece *piece) { return (piece->facets.count == 3); }
bool Piece_isRightPlace(const Piece *piece)
{
    return aColors_EqualSet((const aColors *)&piece->facets, (const aColors *)&piece->faces);
}
bool Piece_isSolved(const Piece *piece)
{
    return aColors_Equals((const aColors *)&piece->facets, (const aColors *)&piece->faces);
}
bool Piece_matchFacets(const Piece *piece, const aColors *facets)
{
    return aColors_EqualSet((const aColors *)&piece->facets, facets);
}
bool Piece_matchFaces(const Piece *piece, const aColors *faces, bool matchAll)
{
    return aColors_Contains((const aColors *)&piece->faces, faces, matchAll);
}

bool Piece_matchFacet2Face(const Piece *piece, Colors facet, Colors face)
{
    for (int i = 0; i < piece->facets.count; i++)
        if (piece->facets.colors[i] == facet)
            return piece->faces.colors[i] == face;
    return false;
}


