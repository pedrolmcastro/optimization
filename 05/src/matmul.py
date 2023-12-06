import sys
import numpy
import random
import timeit

from collections.abc import Sequence, Sized


class Matrix:
    """Namespace for the matrix functions"""


    @staticmethod
    def rows(matrix: Sized):
        """Returns the number of rows of the matrix"""
        return len(matrix)

    @classmethod
    def cols(cls, matrix: Sequence[Sized]):
        """Returns the number of columns of the matrix"""

        if (cls.rows(matrix) == 0):
            return 0

        return len(matrix[0])


    @staticmethod
    def random(rows: int, cols: int):
        """Returns a random matrix"""

        if rows < 0 or cols < 0:
            ValueError("Matrix dimensions can't be negative")

        return [[random.uniform(- 100.0, 100.0) for j in range(cols)] for i in range(rows)]

    @staticmethod
    def zeroed(rows: int, cols: int):
        """Returns a zero matrix"""

        if rows < 0 or cols < 0:
            ValueError("Matrix dimensions can't be negative")

        return [[0.0 for j in range(cols)] for i in range(rows)]


    @classmethod
    def naive(cls, left: Sequence[Sequence[float]], right: Sequence[Sequence[float]]):
        """Matrix multiplication using naive implementation"""

        span = cls.rows(right)

        if cls.cols(left) != span:
            raise ValueError("Left columns must match right rows")

        rows = cls.rows(left)
        cols = cls.cols(right)

        product = cls.zeroed(rows, cols)

        for i in range(rows):
            for j in range(cols):
                for k in range(span):
                    product[i][j] += left[i][k] * right[k][j]

        return product

    @classmethod
    def unrolling(cls, left: Sequence[Sequence[float]], right: Sequence[Sequence[float]]):
        """Matrix multiplication using loop unrolling"""

        span = cls.rows(right)

        if cls.cols(left) != span:
            raise ValueError("Left columns must match right rows")

        rows = cls.rows(left)
        cols = cls.cols(right)

        if rows % 4 != 0 or span % 4 != 0 or cols % 4 != 0:
            raise ValueError("Matrices dimensions must be divisible by 4")

        product = cls.zeroed(rows, cols)

        for i in range(rows):
            for j in range(cols):
                for k in range(0, span, 4):
                    product[i][j] += left[i][k + 0] * right[k + 0][j]
                    product[i][j] += left[i][k + 1] * right[k + 1][j]
                    product[i][j] += left[i][k + 2] * right[k + 2][j]
                    product[i][j] += left[i][k + 3] * right[k + 3][j]

        return product

    @classmethod
    def interchange(cls, left: Sequence[Sequence[float]], right: Sequence[Sequence[float]]):
        """Matrix multiplication using loop interchange"""

        span = cls.rows(right)

        if cls.cols(left) != span:
            raise ValueError("Left columns must match right rows")

        rows = cls.rows(left)
        cols = cls.cols(right)

        product = cls.zeroed(rows, cols)

        for i in range(rows):
            for k in range(span):
                for j in range(cols):
                    product[i][j] += left[i][k] * right[k][j]

        return product


    @staticmethod
    def dot(left: numpy.matrix, right: numpy.matrix):
        """Matrix multiplication using numpy.dot"""
        return numpy.dot(left, right)

    @staticmethod
    def matmul(left: numpy.matrix, right: numpy.matrix):
        """Matrix multiplication using numpy.matmul"""
        return numpy.matmul(left, right)

    @staticmethod
    def operator(left: numpy.matrix, right: numpy.matrix):
        """Matrix multiplication using numpy __matmul__ operator"""
        return left @ right


def main():
    REPEAT = 10
    size = int(sys.argv[1]) if len(sys.argv) >= 2 else 100

    # Time the custom functions

    left = Matrix.random(size, size)
    right = Matrix.random(size, size)

    naive = timeit.timeit(lambda: Matrix.naive(left, right), number = REPEAT)
    unrolling = timeit.timeit(lambda: Matrix.unrolling(left, right), number = REPEAT)
    interchange = timeit.timeit(lambda: Matrix.interchange(left, right), number = REPEAT)

    # Time the numpy functions

    left = numpy.matrix(left)
    right = numpy.matrix(right)

    dot = timeit.timeit(lambda: Matrix.dot(left, right), number = REPEAT)
    matmul = timeit.timeit(lambda: Matrix.matmul(left, right), number = REPEAT)
    operator = timeit.timeit(lambda: Matrix.operator(left, right), number = REPEAT)

    print(f"Matrix size: {size} x {size}")
    print(f"Number of executions: {REPEAT}", end = "\n\n")

    print(
         "Function    | Executions Total Time (s) | Mean Time (s)              \n"
         "------------+---------------------------+--------------              \n"
        f"naive       | {naive           :<25.6f} | {naive       / REPEAT :.6f}\n"
        f"unrolling   | {unrolling       :<25.6f} | {unrolling   / REPEAT :.6f}\n"
        f"interchange | {interchange     :<25.6f} | {interchange / REPEAT :.6f}\n"
         "------------+---------------------------+--------------              \n"
        f"dot         | {dot             :<25.6f} | {dot         / REPEAT :.6f}\n"
        f"matmul      | {matmul          :<25.6f} | {matmul      / REPEAT :.6f}\n"
        f"operator    | {operator        :<25.6f} | {operator    / REPEAT :.6f}  "
    )

if __name__ == "__main__":
    main()
