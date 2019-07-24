from unittest import TestCase

from .vec2 import Vec2


class TestVec2(TestCase):
    def test_access(self):
        self.assertEqual(Vec2(10, 15).x, 10)
        self.assertEqual(Vec2(-3, 4).y, 4)

    def test_tuple_access_order_is_expected(self):
        v = Vec2(8, 4)
        self.assertEqual(v.x, v[0])
        self.assertEqual(v.y, v[1])

    def test_vector_add_does_not_modify_original(self):
        v1 = Vec2(8, 4)
        v2 = Vec2(5, 3)
        v3 = v1 + v2
        self.assertEqual(v1, (8, 4))

    def test_vector_add(self):
        v1 = Vec2(8, 4)
        v2 = Vec2(5, 3)
        v3 = v1 + v2
        self.assertEqual(v3, (13, 7))

    def test_vector_sub_does_not_modify_original(self):
        v1 = Vec2(8, 4)
        v2 = Vec2(5, 3)
        v3 = v1 - v2
        self.assertEqual(v1, (8, 4))

    def test_vector_sub(self):
        v1 = Vec2(8, 4)
        v2 = Vec2(5, 3)
        v3 = v1 - v2
        self.assertEqual(v3, (3, 1))

    def test_vector_abs_returns_absolute_value(self):
        self.assertAlmostEqual(Vec2(3, 4).abs(), 5)
        self.assertAlmostEqual(Vec2(-3, 4).abs(), 5)
