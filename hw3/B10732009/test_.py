from timeit import Timer
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

# support function

def almostEqual(a, b):
    return abs(a - b) < 1e-6

def writeFile(path, strs):
    with open(path, 'w') as f:
        for str in strs:
            f.write(str+'\n')

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

def testPerformance():
    setup = '''
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
msize = 1000
m1 = Matrix(msize, msize)
m2 = Matrix(msize, msize)
    '''
    
    naive = Timer('multiply_naive(m1, m2)', setup=setup)
    tile16 = Timer('multiply_tile(m1, m2, 16)', setup=setup)
    tile32 = Timer('multiply_tile(m1, m2, 32)', setup=setup)
    tile64 = Timer('multiply_tile(m1, m2, 64)', setup=setup)
    mkl = Timer('multiply_mkl(m1, m2)', setup=setup)

    repeat = 5
    naivesec = min(naive.repeat(repeat=repeat, number=1))
    tile16sec = min(tile16.repeat(repeat=repeat, number=1))
    tile32sec = min(tile32.repeat(repeat=repeat, number=1))
    tile64sec = min(tile64.repeat(repeat=repeat, number=1))
    mklsec = min(mkl.repeat(repeat=repeat, number=1))

    path = 'performance.txt'
    strs = ['Method\tTime\tRatio',
            f'naive \t{naivesec}\t{naivesec/naivesec}',
            f'tile16\t{tile16sec}\t{tile16sec/naivesec}',
            f'tile32\t{tile32sec}\t{tile64sec/naivesec}',
            f'tile64\t{tile64sec}\t{tile64sec/naivesec}',
            f'mkl   \t{mklsec}\t{mklsec/naivesec}']
    writeFile(path, strs)