from builtins import int
from random import choices, randint

from rubik.Move import Move
from rubik.Rubik import Rubik
from rubik.Colors import Colors
from rubik.Faces import Faces, Sides
from rubik.Piece import Piece

class Rubik_3x3(Rubik):
    MOVES = "UDRLFB'2"

    _NUM_ROTATIONS = 5
    _NUM_SHUFFLE = 10

    def __init__(self, create_key=None, showMoves = False):
        assert (create_key is not None), "Illegal Rubik creation. Call RubikFactory.create method instead"

        # build cube
        self._cube = []
        # Face index
        self._facePieces = {}
        # cube faces
        self._faces = {}
        self.__buildCube()

        # Show moves for debugging
        self._showMoves = showMoves

    def __buildCube(self):
        # cube list of pieces: 8 corners + 12 edges
        # Create faces indx
        for color in Colors:
            self._facePieces[color] = []
        # Create cube faces
        for face, color in zip(Faces, Colors):
            # Initial faces/colors as ordered in enum declarations
            # WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
            # UP, DOWN, RIGHT, LEFT, FRONT, BACK
            self._faces[face] = color

        # Create pieces
        sideFaces = [self._faces[side] for side in Faces.DOWN.getSides()]
        sideFacesNext = sideFaces[1:] + sideFaces[:1]
        upDown = [self._faces[side] for side in (Faces.UP, Faces.DOWN)]
        # Piece edge, corner
        for side, sideNext in zip(sideFaces, sideFacesNext):
            # Middle edges
            edge = Piece(side, sideNext)
            self._cube.append(edge)
            self._facePieces[side].append(edge)
            self._facePieces[sideNext].append(edge)
            for sideUpDown in upDown:
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
        faceColor = self._faces[face]
        sideColors = [self._faces[side] for side in face.getSides()]

        # Init layout with centerpiece
        layout = [
            [None, None, None],
            [None, str(faceColor), None],
            [None, None, None]
        ]
        for piece in self._facePieces[faceColor]:
            row = 1
            col = 1
            pieceColor = faceColor
            for k,v in piece.items():
                if v == faceColor:
                    pieceColor = k
                elif v == sideColors[Sides.TOP.value]:
                    row = 0
                elif v == sideColors[Sides.BOTTOM.value]:
                    row = 2
                if v == sideColors[Sides.LEFT.value]:
                    col = 0
                elif v == sideColors[Sides.RIGHT.value]:
                    col = 2
            layout[row][col] = str(pieceColor)

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
        for side in Faces.DOWN.getSides():
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
        return "".join(choices(self.MOVES, k = numMoves))

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
            sideFaces = move.face.getSides()
            step = move.step if move.clockWise else -move.step

            # Assign previous face color to current face
            prevFaces = dict(self._faces)
            N = len(sideFaces)
            for i in range(N):
                prevFace = (N + i - step) % N
                prevColor = prevFaces[sideFaces[prevFace]]
                self._faces[sideFaces[i]] = prevColor

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
            # pre-process move
            faceColor = self._faces[move.face]
            sideColors = [self._faces[side] for side in move.face.getSides()]

            # Remove pieces of this rotating face from side faces
            for sideColor in sideColors:
                self._facePieces[sideColor] = [piece for piece in self._facePieces[sideColor] if faceColor not in piece.values()]

            # Rotate pieces && add to new side
            N = len(sideColors)
            step = move.step if move.clockWise else -move.step
            for piece in self._facePieces[faceColor]:
                for facet, face in piece.items():
                    # Only changes facet of sides
                    if face in sideColors:
                        i = sideColors.index(face)
                        nextFace = (N + i + step) % N
                        newFace = sideColors[nextFace]
                        piece[facet] = newFace
                        # Add rotated piece to new face
                        self._facePieces[newFace].append(piece)

            if self._showMoves:
                print(move)
                self.print()

    def _parse(self, strMoves: str) -> list[Move]:
        moves = []

        face = None
        clockWise = True
        step = 1
        for char in strMoves:
            if char in self.MOVES:
                m = self.MOVES.index(char)
                if m < 6:
                    if face is not None:
                        # Save previous move
                        moves.append(Move(face, clockWise, step))
                    face = Faces(m)
                    clockWise = True
                    step = 1
                elif m == 6:
                    clockWise = False
                else:
                    step = 2
        if face is not None:
            # save last move
            moves.append(Move(face, clockWise, step))

        return moves

    def solve(self):
        pass

