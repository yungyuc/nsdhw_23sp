from _mat import Matrix, multiply_naive, multiply_tile, multiply_mkl
import pytest
import time

def test_mat():

    file = open("performance.txt", "a")

    x, y = Matrix(1000, 1000), Matrix(1000, 1000)
    x.initialize()
    y.initialize()

    # naive
    start = time.time()    
    naive_result = multiply_naive(x, y)
    end = time.time()
    file.write(f"multiply_naive takes: {end-start} to finish the computation.")
    
    # tile
    tile_size = 100
    start = time.time()    
    tile_result = multiply_tile(x, y, tile_size)
    end = time.time()
    file.write(f"multiply_tile takes: {end-start} to finish the computation.")

    # mkl
    start = time.time()    
    mkl_result = multiply_mkl(x, y)
    end = time.time()
    file.write(f"multiply_mkl takes: {end-start} to finish the computation.")