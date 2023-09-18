#!/usr/bin/python3


import csv
import sys
import json
import typing
import logging
import pathlib
import subprocess
import collections

import tqdm


class Perf:
    EVENTS = "branch-instructions,branch-misses,L1-dcache-loads,L1-dcache-load-misses"
    FLAGS = f"-x ',' -e {EVENTS} --append"

    # The command uses 'sudo' because 'perf' requires it to measure some events
    COMMAND = f"sudo TIMING_CSV=1 perf stat {FLAGS}"


def main():
    if len(sys.argv) < 4:
        error(f"Usage: python3 {sys.argv[0]} runs path/to/output.json [path/to/executable ...]")


    runs = int(sys.argv[1]) if sys.argv[1].isdigit() else 0

    if runs <= 0:
        error("The number of runs must be a positive integer")


    outpath = pathlib.Path(sys.argv[2])

    if outpath.suffix != ".json":
        error("The output filepath must point to an .json file")

    outpath.parent.mkdir(exist_ok = True)


    cpu = subprocess.check_output("lscpu -J", shell = True)
    date = subprocess.check_output("date").decode().strip()


    logging.info(f"Start profiling: {date}")
    data = []

    for bin in sys.argv[3:]:
        logging.info(f"Profiling {bin}")
        measurements = collections.defaultdict(list)

        for i in tqdm.tqdm(range(runs)):
            result = subprocess.run(f"{Perf.COMMAND} {bin}", capture_output = True, shell = True)

            stdout = result.stdout.decode()
            stderr = result.stderr.decode()

            for measure, _, name, *rest in csv.reader(stderr.splitlines()):
                measurements[name].append(int(measure))

            for name, time in csv.reader(stdout.splitlines()):
                measurements[name].append(float(time))

        data.append({
            "bin": bin,
            "measurements": measurements,
        })


    with outpath.open('w') as output:
        output.write(json.dumps({
            "cpu": json.loads(cpu),
            "date": date,
            "data": data,
        }, indent = 2))


def error(message: str) -> typing.NoReturn:
    logging.error(message)
    sys.exit(1)


if __name__ == "__main__":
    logging.basicConfig(level = logging.INFO)
    main()
