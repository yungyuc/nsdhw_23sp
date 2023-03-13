import _vector
import pytest
import math

abs_error = 0.00001

def test_invalid():
    v1 = (0, 0)
    v2 = (123.87, 456.12)
    res = _vector.cal_rad(v1, v2)

    assert res == -1

def test_0_angle():
    v1 = (123.58, 456.74)
    coef = 0.123
    v2 = tuple([coef*x for x in v1])
    res = _vector.cal_rad(v1, v2)
    print(v1, v2, res)

    assert abs(res - 0) < abs_error

def test_45_angle():
    v1 = (987.12, 0)
    v2 = (456.23, 456.23)
    res = _vector.cal_rad(v1, v2)

    assert abs(res - math.pi/4) < abs_error

def test_90_angle():
    v1 = (123.45, 0)
    v2 = (0, 567.89)
    res = _vector.cal_rad(v1, v2)

    assert abs(res - math.pi/2) < abs_error

def test_180_angle():
    v1 = (123.45, 456.78)
    coef = -27.987
    v2 = tuple([coef*x for x in v1])
    res = _vector.cal_rad(v1, v2)

    assert abs(res - math.pi) < abs_error
