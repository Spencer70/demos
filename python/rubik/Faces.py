from enum import IntEnum

class Colors(IntEnum):
    WHITE, YELLOW, BLUE, GREEN, RED, ORANGE = range(6)

class Sides(IntEnum):
    #  Side faces ordered clockwise as left, top, right, bottom
    LEFT, TOP, RIGHT, BOTTOM = range(4)

class Faces(IntEnum):
    UP, DOWN, RIGHT, LEFT, FRONT, BACK = range(6)

