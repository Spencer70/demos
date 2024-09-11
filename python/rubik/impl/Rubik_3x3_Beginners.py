from rubik.Faces import Colors, Faces
from rubik.impl.Piece import Piece
from rubik.impl.Rubik_3x3 import Rubik_3x3
from functools import partial


class Rubik_3x3_Beginners(Rubik_3x3):
    
    def __init__(self, create_key=None, showMoves=False):
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

    def _fromColors(self, *facets: Colors) -> Piece | None:
        # Search whole cube
        for piece in self._cube:
            if piece.matchColors(*facets):
                return piece
        return None

    def matchFaces(self, piece: Piece, *faces: Faces, matchAll=True) -> bool:
        # Match faces
        faceColors = [self._faceColors[face] for face in faces]
        return piece.matchFaceColors(*faceColors, matchAll=matchAll)

    def _L1L2Stage(self, workingFace: str, findPiece, preparePiece, solvePiece):
        _, sideColors, _ = self._getTopology(Faces.UP)
        prevSideColor = sideColors[-1]
        for sideColor in sideColors:
            piece = findPiece(sideColor, prevSideColor)
            # check piece in right position; do nothing
            if not piece.isSolved():
                # Rotate cube sides to bring piece to front
                while (not self.matchFaces(piece, Faces.FRONT) or
                       piece.isCorner() and not self.matchFaces(piece, Faces.RIGHT)):
                    self._rotate("U")
                frontColor = preparePiece(piece, sideColor)

                # Rotate cube sides to bring sideColor face to front
                while self._faceColors[Faces.FRONT] != frontColor:
                    self._rotate("U")

                # Bring piece to front
                while (not piece.matchFaceColors(frontColor) or
                        piece.isCorner() and not piece.matchFaceColors(prevSideColor)):
                    self._permute(workingFace)

                solvePiece(piece)
            prevSideColor = sideColor

    def _TopCross(self):
        self._L1L2Stage("D", 
                        partial(self._TopCross_findPiece),
                        partial(self._TopCross_preparePiece),
                        partial(self._TopCross_solvePiece))

    def _TopCross_findPiece(self, side: Colors, prev: Colors) -> Piece:
        return self._fromColors(self._faceColors[Faces.UP], side)

    def _TopCross_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send top/middle piece to down
        if self.matchFaces(piece, Faces.UP):
            # piece in top layer
            self._permute("F2")
        elif not self.matchFaces(piece, Faces.DOWN):
            # piece in middle layer
            moves = "FDF'" if self.matchFaces(piece, Faces.RIGHT) else "F'DF"
            self._permute(moves)
        return sideColor

    def _TopCross_solvePiece(self, piece: Piece):
        # Bring piece to up face
        moves = "F2" if piece[self._faceColors[Faces.UP]] == self._faceColors[Faces.DOWN] else "DRF'R'"
        self._permute(moves)

    def _TopCorners(self):
        self._L1L2Stage("D",
                        partial(self._TopCorners_findPiece),
                        partial(self._TopCorners_preparePiece),
                        partial(self._TopCorners_solvePiece))

    def _TopCorners_findPiece(self, side: Colors, prev: Colors) -> Piece:
        return self._fromColors(self._faceColors[Faces.UP], side, prev)

    def _TopCorners_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send top piece to down
        if self.matchFaces(piece, Faces.UP):
            # piece in top layer
            moves = "R'D'R" if self.matchFaces(piece, Faces.RIGHT) else "LDL'"
            self._permute(moves)
        return sideColor

    def _TopCorners_solvePiece(self, piece: Piece):
        # Bring piece to up face
        if piece[self._faceColors[Faces.UP]] == self._faceColors[Faces.DOWN]:
            self._permute("R'D2RDR'D'R")
        elif piece[self._faceColors[Faces.UP]] == self._faceColors[Faces.FRONT]:
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
        return self._fromColors(side, prev)

    def _MiddleEdges_preparePiece(self, piece: Piece, sideColor: Colors) -> Colors:
        # send middle piece to up
        if not self.matchFaces(piece, Faces.UP):
            self._swapEdge(self.matchFaces(piece, Faces.RIGHT))
        facets = list(piece.keys())
        return facets[0] if piece[facets[0]] != self._faceColors[Faces.UP] else facets[1]

    def _MiddleEdges_solvePiece(self, piece: Piece):
        self._swapEdge(piece.get(self._faceColors[Faces.RIGHT]) == self._faceColors[Faces.UP])

    def _swapEdge(self, toRight: bool):
        if toRight:
            self._permute("URU'R'U'F'UF")
        else:
            self._permute("U'L'ULUFU'F'")

    def _LLCross(self):
        # Search last layer
        upColor = self._faceColors[Faces.UP]
        LLEdges = [piece for piece in self._facePieces[upColor] if piece.isEdge()]

        # Repeat until upcolor cross solved.
        # Find cross pieces with solved upcolor
        while len(crossPieces := [piece for piece in LLEdges if piece[upColor] == upColor]) < 4:
            if len(crossPieces) == 2:
                # Order found pieces
                while (leftPiece := self._findSemiLLCross(crossPieces)) is None:
                    self._rotate("U")
                # Match semi-cross to left side color
                while leftPiece.get(self._faceColors[Faces.LEFT]) != self._faceColors[Faces.LEFT]:
                    self._permute("U'")
                    self._rotate("U")
            # Try to solve 4 cross pieces to upcolor
            self._permute("FRUR'U'F'")

    def _findSemiLLCross(self, crossPieces: list[Piece]) -> Piece | None:
        if any(self.matchFaces(piece, Faces.BACK, Faces.RIGHT, matchAll=False) for piece in crossPieces):
            for piece in crossPieces:
                if self.matchFaces(piece, Faces.LEFT):
                    return piece
        return None

    def _LLSolvedCross(self):
        # Search last layer
        upColor = self._faceColors[Faces.UP]
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
        back_or_right = any(self.matchFaces(piece, Faces.BACK, Faces.RIGHT, matchAll=False) for piece in crossPieces)
        left = any(self.matchFaces(piece, Faces.LEFT) for piece in crossPieces)
        return back_or_right and left

    def _LLCorners(self):
        # Search last layer
        upColor = self._faceColors[Faces.UP]
        LLCorners = [piece for piece in self._facePieces[upColor] if piece.isCorner()]

        # while not all corners in right place
        while len(RPCorners := [piece for piece in LLCorners if piece.isRightPlace()]) < 4:
            if len(RPCorners) > 0:
                while not self.matchFaces(RPCorners[0], Faces.FRONT, Faces.RIGHT):
                    self._rotate("U")
            self._permute("URU'L'UR'U'L")

    def _LLSolvedCorners(self):
        # Search last layer
        upColor = self._faceColors[Faces.UP]
        LLCornersNotSolved = [piece for piece in self._facePieces[upColor]
                              if piece.isCorner() and not piece.isSolved()]
        if len(LLCornersNotSolved) > 0:
            # Order 1rst corner not solved
            while not self.matchFaces(LLCornersNotSolved[0], Faces.FRONT, Faces.RIGHT):
                self._rotate("U")

            for piece in LLCornersNotSolved:
                while not self.matchFaces(piece, Faces.FRONT, Faces.RIGHT):
                    self._permute("U")

                # Solve piece
                while piece[upColor] != upColor:
                    self._permute("R'D'RD")

            # Permute LL to match sides
            while not LLCornersNotSolved[0].isSolved():
                self._permute("U")
