import math

import pytest

import _vector

_angle = _vector.angle_between_vectors

def test_zero_magnitude():
    assert math.isnan(_angle((0,0),(0,0))) == True

    assert math.isnan(_angle((1,2),(0,0))) == True

    assert math.isnan(_angle((0,0),(1,2))) == True


def test_two_dimension():
    with pytest.raises(TypeError):
        _angle((),())

    with pytest.raises(TypeError):
        _angle((3),(5,7))

    with pytest.raises(TypeError):
        _angle((1,2),(3,4,5))


def test_correct_angle():
    assert _angle((1,1),(2,2)) == pytest.approx(0, abs=1e-6)

    assert _angle((1,0),(2,2)) == pytest.approx(math.pi/4, abs=1e-6)

    assert _angle((2,1),(1,-2)) == pytest.approx(math.pi/2, abs=1e-6)

    assert _angle((1,0),(-1,0)) == pytest.approx(math.pi, abs=1e-6)

    assert _angle((1,0),(1,math.sqrt(3))) == pytest.approx(math.pi/3, abs=1e-6)
