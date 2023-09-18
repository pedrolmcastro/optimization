#!/usr/bin/python3


import sys
import json
import typing
import logging
import pathlib
import statistics

import scipy.stats as st


def main():
    CONFIDENCE = 0.95
    DIVISION = f"{'-' * 25}+{'-' * 30}+{'-' * 15}"


    if len(sys.argv) < 2:
        abort(f"Usage: python3 {sys.argv[0]} path/to/input.json")

    filepath = pathlib.Path(sys.argv[1])

    if not filepath.is_file() or filepath.suffix != ".json":
        abort(f"The path '{filepath}' is not a valid .json file")


    with filepath.open() as file:
        data = json.load(file)["data"]


    for filepath, measurements in data.items():
        print(f"{unmangle(filepath)}:")
        print(DIVISION)

        for metric, values in measurements.items():
            mean = statistics.mean(values)
            median = statistics.median(values)

            error = st.sem(values)
            delta = error * st.t.ppf((CONFIDENCE + 1) / 2, len(values) - 1)

            print(f"{metric :<24} | {mean :<15.2f} ± {delta :<10.2f} | {median :.2f}")

        print(f"{DIVISION}\n")


def abort(message: str) -> typing.NoReturn:
    logging.error(message)
    sys.exit(1)


def unmangle(filepath: str) -> str:
    """Retorna um nome legível derivado do caminho do arquivo"""

    if "naive" in filepath:
        algorithm = "Sem otimização"
    elif "unrolling" in filepath:
        algorithm = "Loop unrolling"
    elif "interchange" in filepath:
        algorithm = "Loop interchange"

    if "static" in filepath:
        allocation = "Estática"
    elif "unidim" in filepath:
        allocation = "Dinâmica unidimensional"
    elif "bidim" in filepath:
        allocation = "Dinâmica bidimensional"

    return f"{allocation} + {algorithm}"


if __name__ == "__main__":
    main()
