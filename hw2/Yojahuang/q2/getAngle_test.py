import _vector
import math
import pytest

def test_get_angle():
  v = [[1, 0], [0, 1], [-1, 0],  [0, -1]]  
  assert math.isclose(_vector.getAngle(v[0], v[0]), 0)

  assert math.isclose(_vector.getAngle(v[0], v[1]), math.pi/2)
  assert math.isclose(_vector.getAngle(v[0], v[2]), math.pi)
  assert math.isclose(_vector.getAngle(v[0], v[3]), math.pi/2)
  assert math.isclose(_vector.getAngle(v[1], v[2]), math.pi/2)
  assert math.isclose(_vector.getAngle(v[1], v[3]), math.pi)
  assert math.isclose(_vector.getAngle(v[2], v[3]), math.pi/2)
