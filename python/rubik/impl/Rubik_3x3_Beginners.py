from rubik.Colors import Colors
from rubik.Faces import Faces
from rubik.Piece import Piece
from rubik.impl.Rubik_3x3 import Rubik_3x3
from functools import partial

class Rubik_3x3_Beginners(Rubik_3x3):
    
    def __init__(self, create_key=None, showMoves = False):
        super().__init__(create_key, showMoves)

    def solve(self):
        # Top layer
        self._TopCross()
        self._TopCorners()

        self._MiddleFlip()

        # Middle layer
        self._MiddleEdges()

        # Last layer
        self._LLCross()
        self._LLSolvedCross()
        self._LLCorners()
        self._LLSolvedCorners()

        self._MiddleFlip()

    def _fromFacets(self, *facets: Colors) -> Piece | None:
        # Search whole cube
        for piece in self._cube:
            if piece.match(*facets):
                return piece
        return None

    def _L1L2Stage(self, workingFace: str, findPiece, preparePiece, solvePiece):
        sideColors = [self._faces[face] for face in Faces.UP.getSides()]
        prevSideColor = sideColors[-1]
        for sideColor in sideColors:
            piece = findPiece(sideColor, prevSideColor)
            # check piece in right position; do nothing
            if not piece.isSolved():
                # Rotate cube sides to bring piece to front
                while (self._faces[Faces.FRONT] not in piece.values() or
                        piece.isCorner() and self._faces[Faces.RIGHT] not in piece.values()):
                    self._rotate("U")

                frontColor = preparePiece(piece, sideColor)

                # Rotate cube sides to bring sideColor face to front
                while (self._faces[Faces.FRONT] != frontColor):
                    self._rotate("U")

                # Bring piece to front
                while (frontColor not in piece.values() or
                        piece.isCorner() and prevSideColor not in piece.values()):
                    self._permute(workingFace)

                solvePiece(piece)
            prevSideColor = sideColor

    def _TopCross(self):
        self._L1L2Stage("D", 
                        partial(self._TopCross_findPiece),
                        partial(self._TopCross_preparePiece),
                        partial(self._TopCross_solvePiece))

    def _TopCross_findPiece(self, side: Colors, prev: Colors) -> Piece:
        return self._fromFacets(self._faces[Faces.UP], side)

    def _TopCross_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send top/middle piece to down
        if self._faces[Faces.UP] in piece.values():
            # piece in top layer
            self._permute("F2")
        elif self._faces[Faces.DOWN] not in piece.values():
            # piece in middle layer
            moves = "FDF'" if self._faces[Faces.RIGHT] in piece.values() else "F'DF"
            self._permute(moves)
        return sideColor

    def _TopCross_solvePiece(self, piece: Piece):
        # Bring piece to up face
        moves = "F2" if piece[self._faces[Faces.UP]] == self._faces[Faces.DOWN] else "DRF'R'"
        self._permute(moves)

    def _TopCorners(self):
        self._L1L2Stage("D",
                        partial(self._TopCorners_findPiece),
                        partial(self._TopCorners_preparePiece),
                        partial(self._TopCorners_solvePiece))

    def _TopCorners_findPiece(self, side: Colors, prev: Colors) -> Piece:
        return self._fromFacets(self._faces[Faces.UP], side, prev)

    def _TopCorners_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send top piece to down
        if self._faces[Faces.UP] in piece.values():
            # piece in top layer
            moves = "R'D'R" if self._faces[Faces.RIGHT] in piece.values() else "LDL'"
            self._permute(moves)
        return sideColor

    def _TopCorners_solvePiece(self, piece: Piece):
        # Bring piece to up face
        if piece[self._faces[Faces.UP]] == self._faces[Faces.DOWN]:
            self._permute("R'D2RDR'D'R")
        elif piece[self._faces[Faces.UP]] == self._faces[Faces.FRONT]:
            self._permute("FDF'")
        else:
            self._permute("R'D'R")

    def _MiddleFlip(self):
        # Rotate cube upside down
        self._rotate("R2")

    def _MiddleEdges(self):
        self._L1L2Stage("U",
                        partial(self._MiddleEdges_findPiece),
                        partial(self._MiddleEdges_preparePiece),
                        partial(self._MiddleEdges_solvePiece))

    def _MiddleEdges_findPiece(self, side: Colors, prev: Colors) -> Piece:
        return self._fromFacets(side, prev)

    def _MiddleEdges_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send middle piece to up
        if self._faces[Faces.UP] not in piece.values():
            self._swapEdge(self._faces[Faces.RIGHT] in piece.values())
        facets = list(piece.keys())
        return facets[0] if piece[facets[0]] != self._faces[Faces.UP] else facets[1]

    def _MiddleEdges_solvePiece(self, piece: Piece):
        self._swapEdge(piece.get(self._faces[Faces.RIGHT]) == self._faces[Faces.UP])

    def _swapEdge(self, toRight: bool):
        if toRight:
            self._permute("URU'R'U'F'UF")
        else:
            self._permute("U'L'ULUFU'F'")

    def _LLCross(self):
        # Search last layer
        upColor = self._faces[Faces.UP]
        LLEdges = [piece for piece in self._facePieces[upColor] if piece.isEdge()]

        # Repeat until upcolor cross solved.
        # Find cross pieces with solved upcolor
        while len(crossPieces := [piece for piece in LLEdges if piece[upColor] == upColor]) < 4:
            if len(crossPieces) == 2:
                # Order found pieces
                while (leftPiece := self._findSemiLLCross(crossPieces)) is None:
                    self._rotate("U")
                # Match semi-cross to left side color
                while leftPiece.get(self._faces[Faces.LEFT]) != self._faces[Faces.LEFT]:
                    self._permute("U'")
                    self._rotate("U")
            # Try to solve 4 cross pieces to upcolor
            self._permute("FRUR'U'F'")

    def _findSemiLLCross(self, crossPieces: list[Piece])-> Piece | None:
        semiCross = {self._faces[Faces.BACK], self._faces[Faces.RIGHT]}
        if any(len(semiCross.intersection(piece.values())) for piece in crossPieces):
            for piece in crossPieces:
                if self._faces[Faces.LEFT] in piece.values():
                    return piece
        return None

    def _LLSolvedCross(self):
        # Search last layer
        upColor = self._faces[Faces.UP]
        LLEdges = [piece for piece in self._facePieces[upColor] if piece.isEdge()]

        # Repeat until cross is solved.
        while True:
            # there always are 2 solved cross pieces just moving upface
            while len(crossSolvedPieces := [piece for piece in LLEdges if piece.isSolved()]) < 2:
                self._permute("U")
            if len(crossSolvedPieces) == 4:
                break

            # Order solved pieces
            while not self._findSemiLLSolvedCross(crossSolvedPieces):
                self._rotate("U")
            # Try to solve 4 cross pieces
            self._permute("URUR'URU2R'")

    def _findSemiLLSolvedCross(self, crossPieces: list[Piece]) -> bool:
        back_or_right = any(self._faces[Faces.BACK] in piece.values() or self._faces[Faces.RIGHT] in piece.values() for piece in crossPieces)
        left = any(self._faces[Faces.LEFT] in piece.values() for piece in crossPieces)
        return back_or_right and left

    def _LLCorners(self):
        # Search last layer
        upColor = self._faces[Faces.UP]
        LLCorners = [piece for piece in self._facePieces[upColor] if piece.isCorner()]

        # while not all corners in right place
        while len(RPCorners := [piece for piece in LLCorners if piece.isRightPlace()]) < 4:
            if len(RPCorners) > 0:
                while not (self._faces[Faces.FRONT] in RPCorners[0].values() and
                           self._faces[Faces.RIGHT] in RPCorners[0].values()):
                    self._rotate("U")
            self._permute("URU'L'UR'U'L")

    def _LLSolvedCorners(self):
        # Search last layer
        upColor = self._faces[Faces.UP]
        LLCornersNotSolved = [piece for piece in self._facePieces[upColor]
                              if piece.isCorner() and not piece.isSolved()]
        if len(LLCornersNotSolved) > 0:
            # Order 1rst corner not solved
            while not (self._faces[Faces.FRONT] in LLCornersNotSolved[0].values() and
                       self._faces[Faces.RIGHT] in LLCornersNotSolved[0].values()):
                self._rotate("U")

            for piece in LLCornersNotSolved:
                while not (self._faces[Faces.FRONT] in piece.values() and
                           self._faces[Faces.RIGHT] in piece.values()):
                    self._permute("U")

                 # Solve piece
                while piece[upColor] != upColor:
                    self._permute("R'D'RD")

            # Permute LL to match sides
            while not LLCornersNotSolved[0].isSolved():
                self._permute("U")


