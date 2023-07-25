from rubik.Rubik import Rubik
from rubik.impl.Rubik_3x3_Beginners import Rubik_3x3_Beginners
from rubik.impl.Rubik_3x3_Test import Rubik_3x3_Test


class RubikFactory():
    create_key = "RubikFactory"

    @classmethod
    def create(cls, test: bool = False, show: bool = False, dimension: int = 3) -> Rubik:
        assert (dimension == 3), "Only 3x3 cube is implemented"

        return Rubik_3x3_Test(cls.create_key, show) if test else Rubik_3x3_Beginners(cls.create_key, show)

