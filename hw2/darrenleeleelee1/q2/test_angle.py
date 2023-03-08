#!/usr/bin/env python3
import _vector
import pytest
def test_angle_between_vectors():
    v1 = [1.0, 0.0]
    v2 = [0.0, 1.0]
    print(_vector.angle_between_vectors(v1, v2))
    assert _vector.angle_between_vectors(v1, v2) == pytest.approx(1.5708, abs=1e-4)