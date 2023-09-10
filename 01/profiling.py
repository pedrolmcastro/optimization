#!/usr/bin/python3

import sys
import csv
import json
import importlib.util
from subprocess import check_output, run
from collections import defaultdict

runs = 10

perf_events="branch-instructions,branch-misses,L1-dcache-load-misses,L1-dcache-loads"
perf_flags=f"-x ',' -e {perf_events} --append"

def main():
    cpu_info = check_output("lscpu -J", shell=True)
    date = check_output("date").decode().strip()
    start_msg = f"== Starting benchmarking [{date}] =="
    print(start_msg, file=sys.stderr)

    all_measurements = []
    for bin in sys.argv[1:]:
        begin_profile_msg = f"Profiling ${bin}..."
        print(begin_profile_msg, file=sys.stderr)

        measurements = defaultdict(list)
        for i in tqdm(range(runs)):
            result = run(
                f"TIMING_CSV=1 perf stat {perf_flags} {bin}",
                capture_output=True, shell=True)
            stderr = result.stderr.decode()
            stdout = result.stdout.decode()
            for measure, _, name, *rest in csv.reader(stderr.splitlines()):
                measurements[name].append(int(measure))

            for name, time in csv.reader(stdout.splitlines()):
                measurements[name].append(float(time))

        all_measurements.append({
            "bin": bin,
            "measurements": measurements,
        })

    with open("profiling.json", 'w') as prof_file:
        prof_file.write(json.dumps({
        "cpu_info": json.loads(cpu_info),
        "date": date,
        "data": all_measurements,
    }, indent=2))

def load_tqdm():
    global tqdm

    tqdm_spec = importlib.util.find_spec("tqdm")
    if tqdm_spec is not None:
        tqdm = tqdm_spec.loader.load_module().tqdm
    else:
        def my_tqdm(iter, *args, **kwargs):
            length = None
            try: length = len(iter)
            except: pass

            length = "??" if length is None else str(length)
            spinner = "\\|/-";
            for i, el in enumerate(iter):
                print(f"\r{spinner[i%4]} [{i+1}/{length}]", end="", file=sys.stderr)
                yield el

            print()

        tqdm = my_tqdm

if __name__ == '__main__':
    load_tqdm()
    main()
