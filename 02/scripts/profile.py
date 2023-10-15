#! /usr/bin/python3

import re
import sys
import json
import tqdm
import typing
import logging
import pathlib
import subprocess
import pandas as pd
from collections import defaultdict


class Gprof:
    """
    Flags:
        -b : Print data without explanation.
    """
    FLAGS = "-b"

    COMMAND = f"gprof {FLAGS}"

    FLAT_PROFILE_COLS = [
        '% time',
        'cumulative seconds',
        'self seconds',
        'calls',
        'self ms/call',
        'total ms/call',
        'name'
    ]


def main():
    if len(sys.argv) < 4:
        abort(f"Usage: python3 {sys.argv[0]} runs path/to/executable path/to/gmon.out")

    runs = int(sys.argv[1]) if sys.argv[1].isdigit() else 0

    if runs <= 0:
        abort("The number of runs must be a positive integer")

    cpu = subprocess.check_output("lscpu -J", shell = True)
    date = subprocess.check_output("date").decode().strip()

    logging.info(f"Start profiling: {date}")

    executions = defaultdict(float)

    subprocess.run("mkdir -p out/jsons out/reports", capture_output = True, shell = True)

    for i in tqdm.tqdm(range(runs)):
        subprocess.run(f"{sys.argv[2]}", capture_output = True, shell = True)
        result = subprocess.run(f"{Gprof.COMMAND} {sys.argv[2]} {sys.argv[3]}", capture_output = True, shell = True)

        stdout = result.stdout.decode()

        measurements = structure_data(stdout.splitlines())
        executions[i] = measurements

        with open(f"out/reports/{i}.out", "w+") as f:
            f.write(stdout)

    index = pd.MultiIndex.from_product((measurements.keys(), Gprof.FLAT_PROFILE_COLS))

    executions = [tuple([executions[i][j][k] for j,k in index]) for i in range(runs)]
    dataframe = pd.DataFrame(executions, index=range(runs), columns=index)

    outpath = pathlib.Path("out/jsons/time.out")
    outpath_stats = pathlib.Path("out/jsons/stats.out")

    if outpath.suffix != ".out":
        abort("The output filepath must point to an .out file")

    outpath.parent.mkdir(exist_ok = True)

    with outpath.open("w+") as output:
        output.write(
            json.dumps({
            "cpu": json.loads(cpu),
            "date": date,
            "data": json.loads(dataframe.to_json(orient='records')),
        }, indent = 2))

    with outpath_stats.open("w+") as output_stats:
        output_stats.write(
            json.dumps({
            "cpu": json.loads(cpu),
            "date": date,
            "data": json.loads(dataframe.describe().reset_index().to_json(orient='records')),
        }, indent = 2))


def abort(message: str) -> typing.NoReturn:
    logging.error(message)
    sys.exit(1)

def structure_data(data : str) -> defaultdict:
    """
    Gets pure data from gprof output and structures it into a json file
    """
    get_data = False
    measures = defaultdict(float)

    for line in data:
        if re.search("Call graph", line):
            break

        numerical_measures = [float(x) for x in re.findall(r"([0-9]+(?:\.[0-9]+)?)+", line)]
        function_name = list(re.findall(r"([A-Za-z_]+(?:\ \[[0-9]+\])?)", line))
        get_data = (get_data and line != "") or (len(numerical_measures) == 6 and len(function_name) == 1)

        if not get_data:
            continue

        function_name = function_name[0]

        measures[function_name] = \
            {Gprof.FLAT_PROFILE_COLS[i] : (numerical_measures[i] if i < len(numerical_measures) else 0.00)
            for i in range(len(Gprof.FLAT_PROFILE_COLS))}

    return measures


if __name__ == "__main__":
    logging.basicConfig(level = logging.INFO)
    main()
