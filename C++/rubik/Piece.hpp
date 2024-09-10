#include <map>

#include "Faces.hpp"

// Pieces have 2 or 3 facets/faces
// Piece facets are invariant in colors. Facets are keys of map
// Piece faces are referenced to centerpieces colors(Invariant for rotations). Faces are values of map
struct Piece : public std::map<Colors, Colors>
{
    // Create ordered: facets and faces are the same color.
    void Init(const std::vector<Colors> &facets);

    // Piece assertions
    bool isEdge() const;
    bool isCorner() const;
    bool isRightPlace() const;
    bool isSolved() const;
    bool matchFacets(const std::vector<Colors> &facets) const;
    bool matchFaces(const std::vector<Colors> &faces, bool matchAll = true) const;

    // Facet assertion
    bool matchFacet2Face(Colors facet, Colors face) const;
};
