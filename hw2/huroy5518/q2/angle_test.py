import _vector
import math
import pytest

def test_zero_length():
    x = _vector.vector(0, 0)
    y = _vector.vector(1, 0)
    try :
        _vector.angle(x, y)
        assert "Should throw exception"
    except Exception as e:
        pass

def test_zero_angle():
    x = _vector.vector(1, 0)
    y = _vector.vector(1, 0)

    assert _vector.angle(x, y) == float(0)

def test_right_angle():
    x = _vector.vector(0, 1)
    y = _vector.vector(1, 0)
    assert _vector.angle(x, y) == math.pi / 2

def test_other_angle():
    x = _vector.vector(1, 0)
    y = _vector.vector(-1, 0)
    assert _vector.angle(x, y) == math.pi