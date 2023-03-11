import _vector
from math import pi


def feq(x, y, eps=1e-6):
    return abs(x - y) < eps


def test_zero_length():
    assert _vector.angle(0, 0, 0, 0)


def test_zero_angle():
    assert feq(_vector.angle(3, 4, 3, 4), 0.0)


def test_right_angle():
    assert feq(_vector.angle(3, 4, -4, 3), pi / 2.0)


def test_random_angle():
    assert feq(_vector.angle(1, 0, 1, 1),  pi / 4.0)
