#include <algorithm>
#include <vector>

#include "Piece.hpp"

// Create ordered: facets and faces are the same color.
void Piece::Init(const std::vector<Colors> &facets)
{
    for (Colors facet : facets)
        emplace(facet, facet);
}

bool Piece::isEdge()  const { return (size() == 2); }

bool Piece::isCorner() const { return (size() == 3); }

bool Piece::isRightPlace() const
{
    // For each piece face, test piece has a facet of that face
    //auto predicate = [this](std::pair<const Colors, Colors> const& item){ return (this->count(item.second) > 0); };
    auto predicate = [this](auto const& item){ return (this->count(item.second) > 0); };
    return std::all_of(begin(), end(), predicate);
}

bool Piece::isSolved() const
{
    // Test facets are pointing to faces of their color
    auto predicate = [this](auto const& item){ return (item.first == item.second); };
    return std::all_of(begin(), end(), predicate);
}

bool Piece::matchFacets(const std::vector<Colors> &facets) const
{
    // Check equal number of facets
    if (facets.size() != size())
        return false;
    // For each facet, test piece has that facet
    auto predicate = [this](Colors facet){ return (this->count(facet) > 0); };
    return std::all_of(facets.begin(), facets.end(), predicate);
}

bool Piece::matchFaces(const std::vector<Colors> &faces, bool matchAll) const
{
    // Count how many of that faces, this piece has.
    int count = 0;
    for (auto const& it : *this)
        if (std::find(faces.begin(), faces.end(), it.second) != faces.end())
            count++;
    return (matchAll ? count == faces.size() : count > 0);
}

bool Piece::matchFacet2Face(Colors facet, Colors face) const
{
    // Test piece contains that facet and point to face
    auto it = find(facet);
    return (it != end() && it->second == face);
}


