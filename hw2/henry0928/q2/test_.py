import _vector
from math import sqrt, pi

def rad2angle(rad):
    return rad * 180 / pi 

#testcase

def test_angle90():
    test1 = _vector.vec(1,0,0,1)
    test2 = _vector.vec(3,0,0,5)
    test3 = _vector.vec(2,0,0,-2)
    assert rad2angle(test1.angle()) == 90 
    assert rad2angle(test2.angle()) == 90 
    assert rad2angle(test3.angle()) == 90
def test_angle0():
    zero_1 = _vector.vec(1,0,3,0)
    zero_2 = _vector.vec(0,-1,0,-2)
    assert rad2angle(zero_2.angle()) == 0
    assert rad2angle(zero_1.angle()) == 0
def test_angle60():
    sixty = _vector.vec(1,0,1,sqrt(3))
    assert round(rad2angle(sixty.angle())) == 60
def test_invalidinput():
    valid = _vector.vec(0,0,1,2)
    assert valid.angle() == -2 # return "-2" means it is an valid input 