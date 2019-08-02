from collections import namedtuple
from math import sqrt


class Vec2:
    """
    Vector class, while it inherits from NamedTuple, it shouldn't be considered as one
    """
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other)          :  # Note: conflicts with NamedTuple __add__
        return Vec2(self.x + other.x, self.y + other.y)

    def __sub__(self, other        )          :
        return Vec2(self.x - other.x, self.y - other.y)

    def abs(self)         :
        return sqrt(self.x ** 2 + self.y ** 2)


def euclidean_distance(v1      , v2      )         :
    return (v2 - v1).abs()
