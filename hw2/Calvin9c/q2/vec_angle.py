import math
from _vector import vec_angle

import pytest

# _vector.vec_angle(float u_x, float u_y, float v_x, float v_y)

def q2_test():
    # Test 1: zero-length 2-vector(invalid input)
    assert math.isnan(vec_angle(0.0, 0.0, 0.0, 0.0))

    # Test 2: zero angle    
    assert math.isclose(vec_angle(1.0, 0.0, 1.0, 0), 0.0)

    # Test 3: right angle (90-deg)
    assert math.isclose(vec_angle(1.0, 0.0, 0.0, 1.0), 90.0)

    # Test 4: one other angle
    assert math.isclose(vec_angle(1.0, 0.0, 3.0, 4.0), 53.0)
    