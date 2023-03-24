import random
import time

import _matrix

def test_matrix():
    size = [50, 500, 1000, 2000]
    f = open("performance.txt", "a")
    for s in size:
        f.write("size: " + str(s) + "\n")
        m1 = _matrix.Matrix(s, s)
        m2 = _matrix.Matrix(s, s)
        for i in range(s):
            for j in range(s)
                m1[i, j] = i*s+j
                m2[i, j] = i*s+j

        f.write("mkl:\n")
        start = time.time()
        golden = _matrix.multiply_mkl(m1, m2)
        end = time.time()
        f.write(str(start-end) + "\n")
        
        f.write("naive:\n")
        start = time.time()
        m3 = _matrix.multiply_naive(m1, m2)
        end = time.time()
        f.write(str(start-end) + "\n")
        assert m3 == golden

        f.write("tile:\n")
        start = time.time()
        golden = _matrix.multiply_tile(m1, m2, 32)
        end = time.time()
        f.write(str(start-end) + "\n")
        assert m3 == golden




if __name__ == '__main__':
	test_matrix()
	