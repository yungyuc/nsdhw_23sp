import _matrix
import timeit

def test_performance():
    setup = '''
import _matrix

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)
tilenumber = 64

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)
    tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2, tilenumber)', setup=setup)
    repeat = 5
    with open('performance.txt', 'w') as f:
        f.write(f"multiply_naive (repeat={repeat}), take min = ")
        naivesec = minsec = min(naive.repeat(repeat = repeat, number = 1))
        f.write(f'{minsec} second\n')
        f.write(f"multiply_tile (repeat={repeat}), take min = ")
        naivesec = minsec = min(tile.repeat(repeat = repeat, number = 1))
        f.write(f'{minsec} second\n')
        f.write(f"multiply_mkl (repeat={repeat}), take min = ")
        naivesec = minsec = min(mkl.repeat(repeat = repeat, number = 1))
        f.write(f'{minsec} second\n')

