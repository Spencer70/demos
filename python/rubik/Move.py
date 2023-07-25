from rubik.Faces import Faces

class Move:
    def __init__(self, face: Faces, clockWise: bool = True, step: int = 1):
        self.face = face
        # Normalize to allowed step values: 1, 2
        self.step = 2 if step > 1 else 1
        # step = 2 -> normalize to clockwise
        self.clockWise = (step == 2 or clockWise)

    def __str__(self) -> str:
        move = self.face.name[0]
        if not self.clockWise:
            move += "'"
        elif self.step == 2:
            move += str(self.step)
        return move
