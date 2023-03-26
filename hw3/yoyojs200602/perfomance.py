from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
import timeit

def gen_test_matrices(size):

    mat1 = Matrix(size, size)
    mat2 = Matrix(size, size)
    mat3 = Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i+j
            mat2[i, j] = i+j
            mat3[i, j] = 0

    return mat1, mat2, mat3

def benchmark():

    mat_size = 1000
    tile_size = 16
    repeat = 5
    mat1, mat2, _ = gen_test_matrices(mat_size)
    ns = dict(multiply_naive=multiply_naive, multiply_tile=multiply_tile, multiply_mkl=multiply_mkl,
                mat1=mat1, mat2=mat2, tile_size=tile_size)

    naive = timeit.Timer('multiply_naive(mat1, mat2)', globals=ns)
    tile = timeit.Timer('multiply_tile(mat1, mat2, tile_size)', globals=ns)
    mkl = timeit.Timer('multiply_mkl(mat1, mat2)', globals=ns)

    with open('performance.txt', 'w') as outfile:
        naive_time = min(naive.repeat(repeat=repeat, number=1))
        tile_time = min(tile.repeat(repeat=repeat, number=1))
        mkl_time = min(mkl.repeat(repeat=repeat, number=1))
        tile_over_naive = naive_time/tile_time
        mkl_over_naive = naive_time/mkl_time

        outfile.write('Naive method (repeat={}): {} sec'.format(repeat, naive_time))
        outfile.write('Tile method using stride={} (repeat={}): {} sec'.format(tile_size, repeat, tile_time))
        outfile.write('mkl method (repeat={}): {} sec\n'.format(repeat, mkl_time))

        outfile.write('Tile speed-up over naive: {} x'.format(tile_over_naive))
        outfile.write('mkl speed-up over naive: {} x'.format(mkl_over_naive))

if __name__ == '__main__':
    benchmark()
