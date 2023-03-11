import numpy as np
import numpy.ma as ma
import math

import _vector


def unit_vector(vector):
    """ Returns the unit vector of the vector.  """
    return vector / np.linalg.norm(vector)


def angle_between(v1, v2):
    """ Returns the angle in radians between vectors 'v1' and 'v2'.  """
    v1_u = unit_vector(v1)
    v2_u = unit_vector(v2)
    return np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))


def feq(x, y):
    return ma.allclose(x, y)


def test_zero_length():
    try:
        _vector.angle_between(0, 0, 0, 0)
        assert False
    except:
        assert True


def test_zero_angle():
    for _ in range(10):
        v = np.random.rand(2)
        assert feq(_vector.angle_between(v, v), 0.0)


def test_right_angle():
    RIGHT_ANGLE = math.pi / 2.0
    for _ in range(10):
        v1 = np.random.rand(2)
        v2 = np.zeros_like(v1)
        v2[0], v2[1] = -v1[1], v1[0]
        assert feq(_vector.angle_between(v1, v2), RIGHT_ANGLE)


def test_random_angle():
    for _ in range(10):
        v1 = np.random.rand(2)
        v2 = np.random.rand(2)
        assert feq(_vector.angle_between(v1, v2),  angle_between(v1, v2))
