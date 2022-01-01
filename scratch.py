from enum import Enum, auto

class SomethingNew(Enum):
    this = auto()
    that = auto()


test_thing = SomethingNew.this
print(type(test_thing))

print(type(1))