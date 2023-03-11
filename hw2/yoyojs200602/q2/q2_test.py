from _vector import calculate_angle
from math import sqrt, pi
import pytest

v0 = [0, 0]
v1 = [1, 0]
v2 = [1, 1]
v3 = [0, 1]
v4 = [0, 2]

def Rad2Angle(rad):
    angle = rad * 180 / pi
    if (angle - round(angle)) < 1e-5:
        return round(angle)
    return angle



def test_invalid_input():
    assert calculate_angle(v0, v1) == -4
    assert calculate_angle(v1, v0) == -4
    assert calculate_angle(v0, v0) == -4

def test_0_angle():
    assert calculate_angle(v1, v1) == 0
    assert calculate_angle(v3, v4) == 0

def test_45_angle():
    assert Rad2Angle(calculate_angle(v1, v2)) == 45
    assert Rad2Angle(calculate_angle(v2, v1)) == 45

def test_90_angle():
    assert Rad2Angle(calculate_angle(v1, v3)) == 90
    assert Rad2Angle(calculate_angle(v3, v1)) == 90

