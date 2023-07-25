
from enum import Enum

def static_init(cls):
    if getattr(cls, "static_init", None):
        cls.static_init()
    return cls

@static_init
class Faces(Enum):
    # Initial position:
    # WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
    UP, DOWN, RIGHT, LEFT, FRONT, BACK = range(0, 6)

    @classmethod
    def static_init(cls):
        #  Define cube topology. sides are clockwise from face
        setattr(cls.UP, 'sides', (cls.LEFT, cls.BACK, cls.RIGHT, cls.FRONT))
        setattr(cls.UP, 'opposite', cls.DOWN)
        setattr(cls.DOWN, 'sides', (cls.LEFT, cls.FRONT, cls.RIGHT, cls.BACK))
        setattr(cls.DOWN, 'opposite', cls.UP)
        setattr(cls.RIGHT, 'sides', (cls.FRONT, cls.UP, cls.BACK, cls.DOWN))
        setattr(cls.RIGHT, 'opposite', cls.LEFT)
        setattr(cls.LEFT, 'sides', (cls.BACK, cls.UP, cls.FRONT, cls.DOWN))
        setattr(cls.LEFT, 'opposite', cls.RIGHT)
        setattr(cls.FRONT, 'sides', (cls.LEFT, cls.UP, cls.RIGHT, cls.DOWN))
        setattr(cls.FRONT, 'opposite', cls.BACK)
        setattr(cls.BACK, 'sides', (cls.RIGHT, cls.UP, cls.LEFT, cls.DOWN))
        setattr(cls.BACK, 'opposite', cls.FRONT)

    def getSides(self):
        return self.sides

    def opposite(self):
        return self.opposite

#  Side faces ordered clockwise as left, top, right, bottom
class Sides(Enum):
    LEFT, TOP, RIGHT, BOTTOM = range(0, 4)
