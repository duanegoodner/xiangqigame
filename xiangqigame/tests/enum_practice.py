from enum import Enum, auto


class MyThings(Enum):
    FIRST = auto()
    SECOND = auto()


print(MyThings.FIRST.value)


