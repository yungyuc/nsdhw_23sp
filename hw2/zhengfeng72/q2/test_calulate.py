import numpy as np
import pytest
import math
import _vector

# Test for zero-length 2-vector (invalid input).
def test_zero_length():
    v1 = np.array([0.0, 0.0])
    v2 = np.array([1.0, 0.0])
    angle = _vector.cal_vec_angle(v1, v2)

    assert angle==-1

# Test for zero angle.
def test_right_angle():
    v1 = np.array([1.0, 0.0])
    v2 = np.array([2.0, 0.0])
    angle = _vector.cal_vec_angle(v1, v2)

    assert abs(angle - math.pi) < 0.001

# Test for right angle (90-deg).
def test_right_angle():
    v1 = np.array([0.0, 1.0])
    v2 = np.array([1.0, 0.0])
    angle = _vector.cal_vec_angle(v1, v2)

    assert abs(angle - math.pi/2) < 0.001

# Test for one other angle.
def test_180_angle():
    v1 = np.array([0.0, 1.0])
    v2 = np.array([0.0, -1.0])
    angle = _vector.cal_vec_angle(v1, v2)

    assert abs(angle - math.pi) < 0.001

