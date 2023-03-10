#!/usr/bin/env python3

import math
import pytest
from cal_vector_angle import calculate_vector_angle

def test_zero_length():
    with pytest.raises(ValueError) as error_info:
        calculate_vector_angle(0,0,0,0)
        assert str(error_info.value) == "zero-length 2-vector"

def test_zero_angle():
    angle_result = calculate_vector_angle(1,0,1,0)
    assert math.isclose(int(angle_result),0)

def test_right_angle():
    angle_result = calculate_vector_angle(1,0,0,1)
    assert math.isclose(int(angle_result),90)

def test_other_angle():
    angle_result = calculate_vector_angle(1,0,-1,0)
    assert math.isclose(int(angle_result),180)

