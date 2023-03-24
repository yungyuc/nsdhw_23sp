import _matrix
import timeit

def benchmark():
    setup = '''
import _matrix
size = 1000
mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)
for i in range(size):
    for j in range(size):
        mat1[i, j] = 2 * (i ** 2) + j + 2.0
        mat2[i, j] = 2 * (j ** 2) + i + 2.0
'''

    repeat = 5

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 16)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

    with open('performance.txt', 'w') as f:
        f.write('Start multiply_naive (repeat=5), take min =')
        naiveTime = min(naive.repeat(repeat=repeat, number=1))
        f.write('{} seconds\n'.format(naiveTime))

        f.write('Start multiply_tile (repeat=5), take min =')
        tileTime = min(tile.repeat(repeat=repeat, number=1))
        f.write('{} seconds\n'.format(tileTime))
        
        f.write('Start multiply_mkl (repeat=5), take min =')
        mklTime = min(mkl.repeat(repeat=repeat, number=1))
        f.write('{} seconds\n'.format(mklTime))

        f.write('Tile speed-up over naive: {} x\n'.format(tileTime / naiveTime))
        f.write('MKL speed-up over naive: {} x\n'.format(mklTime / naiveTime))

if __name__ == "__main__":
    benchmark()