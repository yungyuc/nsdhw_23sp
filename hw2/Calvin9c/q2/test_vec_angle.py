import math
from _vector import vec_angle

import pytest

# _vector.vec_angle(float u_x, float u_y, float v_x, float v_y)

# def test_q2():
#     # Test 1: zero-length 2-vector(invalid input)
#     assert math.isnan(vec_angle(0.0, 0.0, 0.0, 0.0))

#     # Test 2: zero angle    
#     assert math.isclose(vec_angle(1.0, 0.0, 1.0, 0.0), 0.0)

#     # Test 3: right angle (90-deg)
#     assert math.isclose(int(vec_angle(1.0, 0.0, 0.0, 1.0)), int(0.5*math.pi))

#     # Test 4: one other angle
#     assert math.isclose(int(vec_angle(1.0, 0.0, -1.0, 0.0)), int(math.pi))


def test_1():
    # Test 1: zero-length 2-vector(invalid input)
    assert math.isnan(vec_angle(0.0, 0.0, 0.0, 0.0))
def test_2():
    # Test 2: zero angle    
    assert math.isclose(vec_angle(1.0, 0.0, 1.0, 0.0), 0.0)
def test_3():
    # Test 3: right angle (90-deg)
    assert math.isclose(int(vec_angle(1.0, 0.0, 0.0, 1.0)), int(0.5*math.pi))
def test_4():
    # Test 4: one other angle
    assert math.isclose(int(vec_angle(1.0, 0.0, -1.0, 0.0)), int(math.pi))