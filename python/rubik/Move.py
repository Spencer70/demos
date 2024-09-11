
from rubik.Faces import Faces
from rubik.Rubik import Rubik


class Move:
    # Just parse first move from str
    def __init__(self, strMove: str):
        self.face = Faces(Rubik.MOVES.index(strMove[0]))
        if len(strMove) > 1 and (i := Rubik.STEPS.find(strMove[1])) >= 0:
            self.step = 2 + i
        else:
            self.step = 1

    def __str__(self) -> str:
        move = Rubik.MOVES[self.face]
        if (i := self.step - 2) >= 0:
            move += Rubik.STEPS[i]
        return move
