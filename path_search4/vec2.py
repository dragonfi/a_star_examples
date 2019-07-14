from collections import namedtuple
from math import sqrt
from typing import NamedTuple

# TODO(python3.7): Vec2 in typing
class Vec2(NamedTuple):
    """
    Vector class, while it inherits from NamedTuple, it shouldn't be considered as one
    """
    x: float
    y: float

    def __add__(self, other) -> "Vec2":
        return Vec2(self.x + other.x, self.y + other.y)

    def __sub__(self, other) -> "Vec2":
        return Vec2(self.x - other.x, self.y - other.y)

    def abs(self) -> float:
        return sqrt(self.x ** 2 + self.y ** 2)


def euclidean_distance(v1: Vec2, v2: Vec2) -> float:
    return (v2 - v1).abs()
