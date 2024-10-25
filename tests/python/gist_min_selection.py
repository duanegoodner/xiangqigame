from dataclasses import dataclass


@dataclass
class MyObject:
    name: str
    value: int

    def __lt__(self, other):
        return self.value < other.value


if __name__ == "__main__":
    my_obj_a = MyObject("A", 1)
    my_obj_b = MyObject("B", 1)

    result = min(my_obj_b, my_obj_a)
    print(result)
