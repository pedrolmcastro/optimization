#! /usr/bin/python3

import re
import sys
import csv
import json
import time
import typing
import logging
import pathlib
import subprocess
import pandas as pd
from collections import defaultdict
from io import StringIO

import tqdm

class Gprof:
    """
    Flags:
        -z : Show all, even unused, functions.
        -c : Show what functions were never called.
        -b : Print data without explanation.
    """
    FLAGS = "-z -c -b"

    COMMAND = f"gprof {FLAGS}"

    CALL_GRAPH_COLS = [
        'index',
        '% time',
        'self',
        'children',
        'called',
        'name'
    ]

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
    if len(sys.argv) < 3:
        abort(f"Usage: python3 {sys.argv[0]} path/to/executable")

    runs = int(sys.argv[1]) if sys.argv[1].isdigit() else 0

    if runs <= 0:
        abort("The number of runs must be a positive integer")

    cpu = subprocess.check_output("lscpu -J", shell = True)
    date = subprocess.check_output("date").decode().strip()

    gmon_filename = pathlib.Path("./gmon.out")

    logging.info(f"Start profiling: {date}")
    
    executions = defaultdict(float)

    result = subprocess.run("mkdir -p gprof/reports gprof/outputs", capture_output = True, shell = True)

    for i in tqdm.tqdm(range(runs)):
        _ = subprocess.run(f"{sys.argv[2]}", capture_output = True, shell = True)
        result = subprocess.run(f"{Gprof.COMMAND} {sys.argv[2]} {gmon_filename}", capture_output = True, shell = True)

        stdout = result.stdout.decode()
        stderr = result.stderr.decode()

        measurements = structure_data(stdout.splitlines())
        executions[i] = measurements

        with open(f"./gprof/reports/{i}.out", "w+") as f:
            f.write(stdout)

    index = pd.MultiIndex.from_product((measurements.keys(), Gprof.FLAT_PROFILE_COLS))

    executions = [tuple([executions[i][j][k] for j,k in index]) for i in range(runs)]
    dataframe = pd.DataFrame(executions, index=range(runs), columns=index)
    
    outpath = pathlib.Path("./gprof/outputs/time.out")
    outpath_stats = pathlib.Path("./gprof/outputs/stats.out")

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