import pytest
import _vector
import math

'''
Q2 GRADING 
* Test for zero-length 2-vector (invalid input).
* Test for zero angle.
* Test for right angle (90-deg).
* Test for one other angle.
* To get full point one has to do at least 2 of the above.
'''

def get_angle(x1, y1, x2, y2):
    dot = x1 * x2 + y1 * y2
    mag1 = math.sqrt(x1 ** 2 + y1 ** 2)
    mag2 = math.sqrt(x2 ** 2 + y2 ** 2)
    cos_theta = dot / (mag1 * mag2)
    theta = math.acos(cos_theta)
    return theta

def test_angle_between_vectors():
    # Test 1: zero-length 2-vector(invalid input)
    assert math.isnan(_vector.angleBetweenVectors(0, 0, 0, 1))
    # Test 2: zero angle    
    assert abs(_vector.angleBetweenVectors(1, 2, 1, 2) - 0) < 1e-6
    # Test 3: right angle (90-deg)
    assert abs(_vector.angleBetweenVectors(1, 0, 0, 1) - 1.5708) < 1e-4
    # Test 4: one other angle
    assert abs(_vector.angleBetweenVectors(1, 2, -1, 2) - 0.9273) < 1e-4
    

