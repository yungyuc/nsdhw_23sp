import pytest
from _matrix import Matrix

def test_empty_matrix():
    try:
        A = Matrix(0, 1)
        A = Matrix(1, 0)
        A = Matrix(0, 0)
        assert False
    except:
        assert True