
# Pieces are invariant in colors
# Pieces faces are referenced to centerpieces colors(Invariant for rotations)
from rubik.Colors import Colors

class Piece(dict):
    # Create ordered: every piece facet (key) in face of that color (value)
    def __init__(self, *facets: Colors):
        super().__init__({facet: facet for facet in facets})

    def isEdge(self) -> bool:
        return (len(self) == 2)

    def isCorner(self) -> bool:
        return (len(self) == 3)

    def match(self, *facets: tuple[Colors]) -> bool:
        return self.keys() == set(facets)

    def isSolved(self) -> bool:
        return all(k == v for k,v in self.items())

    def isRightPlace(self) -> bool:
        # values == keys
        return self.match(*self.values())

