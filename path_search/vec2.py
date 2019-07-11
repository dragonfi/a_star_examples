from collections import namedtuple
from math import sqrt

Vec2Tuple = namedtuple("Vec2", "x y")
class Vec2(Vec2Tuple):
    def __add__(self, other: "Vec2") -> "Vec2":
        return Vec2(self.x + other.x, self.y + other.y)
    def __sub__(self, other: "Vec2") -> "Vec2":
        return Vec2(self.x - other.x, self.y - other.y)
    def abs(self) -> float:
        return sqrt(self.x ** 2 + self.y ** 2)
