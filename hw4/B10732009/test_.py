from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

# test

def testNrowNcol():
    m = Matrix(2, 3)
    assert m.nrow == 2
    assert m.ncol == 3
    
def testSetGetItem():
    m = Matrix(10, 10)
    for i in range(m.nrow):
        for j in range(m.ncol):
            m[i, j] = i + j
    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == i + j

def testEqual():
    m1 = Matrix(2, 3, [0, 1, 2, 3, 4, 5])
    m2 = Matrix(2, 3, [0, 1, 2, 3, 4, 5])
    assert m1 == m2

def testMultiply():
    m1 = Matrix(2, 3, [0, 1, 2, 3, 4, 5])
    m2 = Matrix(3, 2, [0, 1, 2, 3, 4, 5])
    r1 = multiply_naive(m1, m2)
    r2 = multiply_tile(m1, m2, 32)
    r3 = multiply_mkl(m1, m2)
    assert (r1 == r2 and r2 == r3)