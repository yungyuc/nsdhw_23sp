import pytest
import math
import _angle

def test_zero_len():
    try:
        _angle.cal_angle_in_python(0, 0, 0, 0)
    except RuntimeError as error:
        assert str(error) == "One or both input vectors have zero length"
    try:
        _angle.cal_angle_in_python(1, 2, 0, 0)
    except RuntimeError as error:
        assert str(error) == "One or both input vectors have zero length"

def test_zero_angle():
    assert math.isclose(_angle.cal_angle_in_python(1, 10, 1, 10), 0)

def test_right_angle():
    assert math.isclose(_angle.cal_angle_in_python(1, 0, 0, 1), 90*math.pi/180)

def test_other_angle():
    assert math.isclose(_angle.cal_angle_in_python(0, 1, 1, 1), 45*math.pi/180)