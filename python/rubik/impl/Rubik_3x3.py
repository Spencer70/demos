from builtins import int
from random import choices, randint

from rubik.Move import Move
from rubik.Rubik import Rubik
from rubik.Faces import Colors, Faces, Sides
from rubik.impl.Piece import Piece

class Rubik_3x3(Rubik):
    _NUM_ROTATIONS = 5
    _NUM_SHUFFLE = 10

    def __init__(self, create_key=None, showMoves = False):
        assert (create_key is not None), "Illegal Rubik creation. Call RubikFactory.create method instead"
        # Define face colors and topology
        self._faceColors = []
        self._sides = []
        self._opposite = []
        self._topology()
        # build cube pieces
        self._cube = []
        # Face index
        self._facePieces = []
        self._buildCube()
        # Show moves for debugging
        self._showMoves = showMoves

    def _topology(self):
        # Initial faces color as ordered in enum declaration of Colors
        # WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
        # UP, DOWN, RIGHT, LEFT, FRONT, BACK
        for color in Colors:
            self._faceColors.append(color)

        # Define cube topology.
        # Sides of UP, DOWN, RIGHT, LEFT, FRONT, BACK.
        # Clockwise from face
        self._sides = [
            (Faces.LEFT, Faces.BACK, Faces.RIGHT, Faces.FRONT),
            (Faces.LEFT, Faces.FRONT, Faces.RIGHT, Faces.BACK),
            (Faces.FRONT, Faces.UP, Faces.BACK, Faces.DOWN),
            (Faces.BACK, Faces.UP, Faces.FRONT, Faces.DOWN),
            (Faces.LEFT, Faces.UP, Faces.RIGHT, Faces.DOWN),
            (Faces.RIGHT, Faces.UP, Faces.LEFT, Faces.DOWN),

        ]
        # Opposite of UP, DOWN, RIGHT, LEFT, FRONT, BACK
        self._opposite = [
            Faces.DOWN,
            Faces.UP,
            Faces.LEFT,
            Faces.RIGHT,
            Faces.BACK,
            Faces.FRONT
        ]

    def _getTopology(self, face: Faces) -> tuple[Colors, list[Colors], Colors]:
        return (self._faceColors[face],
                [self._faceColors[side] for side in self._sides[face]],
                self._faceColors[self._opposite[face]])

    def _buildCube(self):
        # Faces indexed with centerpiece color. Invariant to rotations
        for color in Colors:
            self._facePieces.append([])

        # cube list of pieces: 8 corners + 12 edges
        # Create pieces
        Up, sideFaces, Down = self._getTopology(Faces.DOWN)
        sideFacesNext = sideFaces[1:] + sideFaces[:1]

        # Piece edge, corner
        for side, sideNext in zip(sideFaces, sideFacesNext):
            # Middle edges
            edge = Piece( side, sideNext)
            self._cube.append(edge)
            self._facePieces[side].append(edge)
            self._facePieces[sideNext].append(edge)
            for sideUpDown in (Up, Down):
                # up, down edges
                edge = Piece(side, sideUpDown)
                self._cube.append(edge)
                self._facePieces[side].append(edge)
                self._facePieces[sideUpDown].append(edge)
                # corners
                corner = Piece(side, sideNext, sideUpDown)
                self._cube.append(corner)
                self._facePieces[side].append(corner)
                self._facePieces[sideNext].append(corner)
                self._facePieces[sideUpDown].append(corner)

    def _draw(self, face: Faces):

        def color2str(color: Colors) -> str:
            # WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
            colorCodes = ("\033[1;37m", "\033[1;33m", "\033[1;34m", "\033[1;32m", "\033[1;31m", "\u001b[1;38;5;166m")
            RESET = "\033[0m"
            return colorCodes[color] + color.name[0] + RESET

        faceColor, sideColors, _ = self._getTopology(face)

        # Init layout with centerpiece
        layout = [
            ["", "", ""],
            ["", color2str(faceColor), ""],
            ["", "", ""]
        ]
        for piece in self._facePieces[faceColor]:
            row = 1
            col = 1
            pieceColor = faceColor
            for facetColor, facetFaceColor in piece.items():
                if facetFaceColor == faceColor:
                    pieceColor = facetColor
                elif facetFaceColor == sideColors[Sides.TOP]:
                    row = 0
                elif facetFaceColor == sideColors[Sides.BOTTOM]:
                    row = 2
                if facetFaceColor == sideColors[Sides.LEFT]:
                    col = 0
                elif facetFaceColor == sideColors[Sides.RIGHT]:
                    col = 2
            layout[row][col] = color2str(pieceColor)

        return layout

    def print(self):
        # print cube faces unfolded:
        # UP
        layout = self._draw(Faces.UP)
        rowWidth = 2 * len(layout[0])
        for row in layout:
            strRow = " ".join(row)
            print(strRow.rjust(rowWidth + len(strRow), " "))

        # LEFT - FRONT - RIGHT - BACK. Sides of DOWN
        layout = [[], [], []]
        for side in self._sides[Faces.DOWN]:
            sideLayout = self._draw(side)
            for row, sideRow in zip(layout, sideLayout):
                row.extend(sideRow)
        for row in layout:
            print(" ".join(row))

        # DOWN
        layout = self._draw(Faces.DOWN)
        for row in layout:
            strRow = " ".join(row)
            print(strRow.rjust(rowWidth + len(strRow), " "))

    def _randomMoves(self, numMoves: int) -> str:
        strMoves = ""
        for move in choices(self.MOVES, k = numMoves):
            strMoves += move
            if (i := randint(1, 3) - 2) >= 0:
                strMoves += Rubik.STEPS[i]
        return strMoves

    def rotate(self, strMoves: str):
        if strMoves == "RANDOM":
            strMoves = self._randomMoves(self._NUM_ROTATIONS)

        # Debug helper
        if (self._showMoves):
            print("Rotation: ", strMoves)

        self._rotate(strMoves)

    def _rotate(self, strMoves: str):
        rotations = self._parse(strMoves)
        for move in rotations:
            sides = self._sides[move.face]
            # Assign previous face color to current face
            prevFaceColors = list(self._faceColors)
            N = len(sides)
            for i in range(N):
                prevFace = (N + i - move.step) % N
                prevColor = prevFaceColors[sides[prevFace]]
                self._faceColors[sides[i]] = prevColor
            if self._showMoves:
                print(move)
                self.print()

    def isSolved(self) -> bool:
        return all(piece.isSolved() for piece in self._cube)

    def shuffle(self, strMoves: str):
        # Generate random moves
        if strMoves == "RANDOM":
            strMoves = self._randomMoves(randint(self._NUM_SHUFFLE, 2 * self._NUM_SHUFFLE))

        # Debug helper
        if (self._showMoves):
            print("Shuffle: ", strMoves)

        self._permute(strMoves)

    def _permute(self, strMoves: str):
        moves = self._parse(strMoves)
        for move in moves:
            faceColor, sideColors, _ = self._getTopology(move.face)
            # Remove pieces of this rotating face from side faces
            for sideColor in sideColors:
                self._facePieces[sideColor] = [piece for piece in self._facePieces[sideColor] if not piece.matchFaceColors(faceColor)]

            # Rotate pieces && add to new side
            for piece in self._facePieces[faceColor]:
                newFaces = piece.permute(faceColor, sideColors, move.step)
                for newFace in newFaces:
                    self._facePieces[newFace].append(piece)

            if self._showMoves:
                print(move)
                self.print()
                print(move)

    def _parse(self, strMoves: str) -> list[Move]:
        moves = []
        i = 0
        while i < len(strMoves):
            moves.append(Move(strMoves[i:]))
            i += 1 if moves[-1].step == 1 else 2
        return moves

    def solve(self):
        pass

