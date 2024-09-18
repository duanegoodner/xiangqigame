from enum import Enum

class Color(Enum):
    RED = 1
    GREEN = 2
    BLUE = 3

# Iterating over all members of the Enum
for color in Color:
    print(color)


