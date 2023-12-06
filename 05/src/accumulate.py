import sys
import numpy
import random
import timeit
import functools

from numpy.typing import ArrayLike
from collections.abc import Sequence, Iterable


class Accumulate:
    """Namespace for the accumulate functions"""


    @staticmethod
    def indices(numbers: Sequence[float]):
        negatives = 0.0
        positives = 0.0

        for i in range(len(numbers)):
            number = numbers[i]

            if number < 0.0:
                negatives += number
            else:
                positives += number

        return negatives, positives

    @staticmethod
    def iterator(numbers: Iterable[float]):
        negatives = 0.0
        positives = 0.0

        for number in numbers:
            if number < 0.0:
                negatives += number
            else:
                positives += number

        return negatives, positives

    @staticmethod
    def filter(numbers: Iterable[float]):
        negatives = 0.0

        for negative in filter(lambda number: number < 0.0, numbers):
            negatives += negative

        positives = 0.0

        for positive in filter(lambda number: number > 0.0, numbers):
            positives += positive

        return negatives, positives

    @staticmethod
    def reduce(numbers: Iterable[float]):
        def accumulator(accumulated: tuple[float, float], number: float):
            negatives, positives = accumulated

            if number < 0.0:
                return negatives + number, positives
            else:
                return negatives, positives + number

        return functools.reduce(accumulator, numbers, (0.0, 0.0))

    @staticmethod
    def sum(numbers: Iterable[float]):
        negatives = sum(number for number in numbers if number < 0.0)
        positives = sum(number for number in numbers if number > 0.0)

        return negatives, positives

    @staticmethod
    def numpysum(numbers: ArrayLike):
        negatives = numpy.sum(numbers, where = numbers < 0.0)
        positives = numpy.sum(numbers, where = numbers > 0.0)

        return negatives, positives


def main():
    REPEAT = 10
    size = int(sys.argv[1]) if len(sys.argv) >= 2 else 1000000

    # Time the custom functions

    numbers = [random.uniform(- 100.0, 100.0) for _ in range(size)]

    sum = timeit.timeit(lambda: Accumulate.sum(numbers), number = REPEAT)
    filter = timeit.timeit(lambda: Accumulate.filter(numbers), number = REPEAT)
    reduce = timeit.timeit(lambda: Accumulate.reduce(numbers), number = REPEAT)
    indices = timeit.timeit(lambda: Accumulate.indices(numbers), number = REPEAT)
    iterator = timeit.timeit(lambda: Accumulate.iterator(numbers), number = REPEAT)

    # Time the numpy function

    numbers = numpy.array(numbers)

    numpysum = timeit.timeit(lambda: Accumulate.numpysum(numbers), number = REPEAT)

    print(f"Array size: {size}")
    print(f"Number of executions: {REPEAT}", end = "\n\n")

    print(
         "Function    | Executions Total Time (s) | Mean Time (s)           \n"
         "------------+---------------------------+--------------           \n"
        f"indices     | {indices         :<25.6f} | {indices  / REPEAT :.6f}\n"
        f"iterator    | {iterator        :<25.6f} | {iterator / REPEAT :.6f}\n"
        f"filter      | {filter          :<25.6f} | {filter   / REPEAT :.6f}\n"
        f"reduce      | {reduce          :<25.6f} | {reduce   / REPEAT :.6f}\n"
        f"sum         | {sum             :<25.6f} | {sum      / REPEAT :.6f}\n"
        f"numpysum    | {numpysum        :<25.6f} | {numpysum / REPEAT :.6f}  "
    )

if __name__ == "__main__":
    main()
