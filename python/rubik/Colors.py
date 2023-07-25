from enum import Enum

#  Define cube colors
class Colors(Enum):
    WHITE = "\033[1;37m"
    YELLOW = "\033[1;33m"
    BLUE = "\033[1;34m"
    GREEN = "\033[1;32m"
    RED = "\033[1;31m"
    ORANGE = "\u001b[1;38;5;166m"

    def __str__(self) -> str:
        # Color end string, color reset
        RESET = "\033[0m"
        return self.value + self.name[0] + RESET

