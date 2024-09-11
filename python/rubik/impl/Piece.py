
# Pieces are invariant in colors
# Pieces faces are referenced to centerpieces colors(Invariant for rotations)
from rubik.Faces import Colors

class Piece(dict):

    # Create ordered: every piece facet (key) in face of that color (value)
    def __init__(self, *facets: Colors):
        super().__init__({facet: facet for facet in facets})

    def isEdge(self) -> bool:
        return (len(self) == 2)

    def isCorner(self) -> bool:
        return (len(self) == 3)

    def isRightPlace(self) -> bool:
        # values == keys
        return self.matchColors(*self.values())

    def isSolved(self) -> bool:
        return all(facet == face for facet, face in self.items())

    def matchColors(self, *facets: Colors) -> bool:
        return self.keys() == set(facets)

    def matchFaceColors(self, *faceColors: Colors, matchAll = True) -> bool:
        # Match faces
        if matchAll:
            return all(faceColor in self.values() for faceColor in faceColors)
        else:
            return any(faceColor in self.values() for faceColor in faceColors)

    def permute(self, faceColor: Colors, sideColors: list[Colors], step: int) -> list[Colors]:
        # Rotate pieces && return new sides
        newFaces = []
        N = len(sideColors)
        for facet, face in self.items():
            # Only changes facet of sides
            if face in sideColors:
                i = sideColors.index(face)
                nextFace = (N + i + step) % N
                newFaces.append(sideColors[nextFace])
                self[facet] = newFaces[-1]
        return newFaces

