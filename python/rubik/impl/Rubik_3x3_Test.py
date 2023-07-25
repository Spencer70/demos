from random import randint

from rubik.Move import Move
from rubik.impl.Rubik_3x3 import Rubik_3x3

class Rubik_3x3_Test(Rubik_3x3):

    def __init__(self, create_key=None, showMoves = False):
        super().__init__(create_key, showMoves)
        # Save moves for test algorithm and debugging
        self.__strMoves = ""

    def shuffle(self, strMoves: str):
        # Generate random moves
        if strMoves == "RANDOM":
            strMoves = self._randomMoves(randint(self._NUM_SHUFFLE, 2 * self._NUM_SHUFFLE))

        # Simplest algorithm: remember moves
        self.__strMoves = strMoves
        # Debug helper
        if (self._showMoves):
            print("Shuffle: ", strMoves)

        self._permute(strMoves)

    def solve(self):
        strMoves = self.__strMoves
        # reverse moves
        moves = []
        for move in reversed(self._parse(strMoves)):
            moves.append(Move(move.face, not move.clockWise, move.step))
        strMoves = "".join([str(m) for m in moves])
        self._permute(strMoves)

