from math import sqrt, pi
from _vector import _angle

# support function

def almostEqual(a, b):
    return abs(a - b) < 1e-6

def degToRad(deg):
    return deg / 180.0 * pi

# test 

def testZeroLength():
    assert _angle([0, 0], [0, 0]) < 0
    assert _angle([1, 2], [0, 0]) < 0
    assert _angle([0, 0], [3, 4]) < 0

def testZeroAngle():
    assert almostEqual(_angle([1, 0], [1, 0]), degToRad(0.0))
    assert almostEqual(_angle([1.2, -3.4], [2.4, -6.8]), degToRad(0.0))

def testRightAngle():
    assert almostEqual(_angle([1, 0], [0, 1]), degToRad(90.0))
    assert almostEqual(_angle([-1, 1], [1, 1]), degToRad(90.0))

def test30Angle():
    assert almostEqual(_angle([1, 0], [sqrt(3), 1]), degToRad(30.0))
