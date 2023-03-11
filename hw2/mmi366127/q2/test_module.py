
from math import sqrt, pi, isnan
from _vector import angle
import pytest

eps = 1e-5

def rad2angle(x):
    return x * 180.0 / pi

tests = [
    ([0, 0], [1, 1], lambda x : isnan(x)),                           # Test for zero-length 2-vector (invalid input).
    ([1234, 5678], [1234, 5678], lambda x : abs(rad2angle(x)) < eps),      # Test for zero angle
    ([0, 1], [9487, 0], lambda x : abs(rad2angle(x) - 90.0) < eps),        # Test for right angle (90-deg).
    ([0, 1], [1, 1], lambda x : abs(rad2angle(x) - 45.0) < eps),           # Test for 45
    ([1, 0], [1, sqrt(3)], lambda x : abs(rad2angle(x) - 60.0) < eps),     # Test for 60
]

def test_invalid_input():
    A, B, is_correct = tests[0]
    ans = angle(A, B)
    assert is_correct(ans) == True
    ans = angle(B, A)
    assert is_correct(ans) == True

def test_zero_angle():
    A, B, is_correct = tests[1]
    ans = angle(A, B)
    assert is_correct(ans) == True
    ans = angle(B, A)
    assert is_correct(ans) == True

def test_right_angle():
    A, B, is_correct = tests[2]
    ans = angle(A, B)   
    assert is_correct(ans) == True
    ans = angle(B, A)
    assert is_correct(ans) == True

def test_45():
    A, B, is_correct = tests[3]
    ans = angle(A, B)   
    assert is_correct(ans) == True
    ans = angle(B, A)
    assert is_correct(ans) == True


