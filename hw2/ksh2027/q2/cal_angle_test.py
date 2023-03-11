import _vector
import math

def test_cal_angle():
    v0 = [0, 0]
    v1 = [1, 0]
    v2 = [0, 1]
    v3 = [-1, 0]
    v4 = [0, -1]
    try:
        _vector.cal_angle(v1, v0)
    except ValueError as msg:
        assert str(msg) == "The length of vector 2 is zero."
    assert math.isclose(_vector.cal_angle(v1, v1), 0)
    assert math.isclose(_vector.cal_angle(v1, v2), math.pi/2)
    assert math.isclose(_vector.cal_angle(v1, v3), math.pi)
    assert math.isclose(_vector.cal_angle(v1, v4), math.pi/2)