#!/usr/bin/env python3
import math
import pytest
import _vector
# For test the C++ output is correct
def pyAngle(v1, v2):
    dot_product = v1[0] * v2[0] + v1[1] * v2[1]
    mag_v1 = math.sqrt(v1[0] ** 2 + v1[1] ** 2)
    mag_v2 = math.sqrt(v2[0] ** 2 + v2[1] ** 2)
    angle = math.acos(dot_product / (mag_v1 * mag_v2))
    return angle

def test_zero_length_vector():
    v1 = [0.0, 0.0]
    v2 = [1.0, 2.0]
    try:
        _vector.angle_between_vectors(v1, v2)
    except RuntimeError as e:
        assert str(e) == "One or both input vectors have zero length"
    else:
        print("Expect runtime error, but not fail")

def test_zero_angle():
    v1 = [1.0, 0.0]
    v2 = [1.0, 0.0]
    assert _vector.angle_between_vectors(v1, v2) == pytest.approx(pyAngle(v1, v2), abs=1e-6)

def test_right_angle():
    v1 = [1.0, 0.0]
    v2 = [0.0, 1.0]
    assert _vector.angle_between_vectors(v1, v2) == pytest.approx(pyAngle(v1, v2), abs=1e-6)

def test_other_angle():
    v1 = [1.0, 1.0]
    v2 = [-1.0, 1.0]
    assert _vector.angle_between_vectors(v1, v2) == pytest.approx(pyAngle(v1, v2), abs=1e-6)
