import sys
import resource
import statistics
import subprocess
import scipy.stats

from pathlib import Path
from dataclasses import dataclass


class Config:
    REPEATS = 15
    SRCDIR = Path("src")
    BINDIR = Path("bin")

    COMPILER = "gcc"
    LIBS = ["-pthread"]
    FLAGS = ["-Werror", "-Wall", "-Wextra", "-pedantic"]


@dataclass(frozen = True)
class Time:
    compilation: float
    execution: float


def main():
    # Check that the configured directories exist

    if not Config.SRCDIR.is_dir():
        error(f"Invalid source directory: {Config.SRCDIR}")

    Config.BINDIR.mkdir(exist_ok = True)


    # Parse the CLI arguments

    if len(sys.argv) < 2:
        error(f"Usage: python {sys.argv[0]} PROGRAM [ARG ...]")

    program = sys.argv[1]
    args = sys.argv[2:]


    # Define the source file and binary paths

    source = Config.SRCDIR / f"{program}.c"

    if not source.is_file():
        error(f"Invalid source file: {source}")

    binary = Config.BINDIR / f"{program}.out"


    # Compile and execute the program in each optimization Config.REPEATS times

    for optimization in ("-O0", "-O1", "-O2", "-O3", "-Os"):
        times: list[Time] = []

        for _ in range(Config.REPEATS):
            times.append(Time(
                compilation = run(
                    Config.COMPILER,
                    *Config.LIBS,
                    *Config.FLAGS,
                    optimization,
                    str(source),
                    "-o",
                    str(binary)
                ),
                execution = run(binary, *args),
            ))

        size = binary.stat().st_size

        print(f"Optimization level: {optimization}")
        print(f"{size} bytes", end = "\n\n")
        print(table(times), end = "\n\n")


def run(*args: str) -> float:
    """Run a subprocess and return its execution time"""
    # Save how much time the subprocess already used
    usage = resource.getrusage(resource.RUSAGE_CHILDREN)
    start = usage.ru_utime + usage.ru_stime

    # Execute the subprocess
    completed = subprocess.run(list(args), stdout = subprocess.DEVNULL)
    completed.check_returncode()

    # Calculate the time spent on the subprocess
    usage = resource.getrusage(resource.RUSAGE_CHILDREN)
    return usage.ru_utime + usage.ru_stime - start


def delta(values: list[float]):
    error = scipy.stats.sem(values)
    return error * scipy.stats.t.ppf((0.95 + 1) / 2, len(values) - 1)


def table(times: list[Time]):
    def line(time: Time) -> str:
        return f"{time.compilation :<21.6f} | {time.execution :.6f}"

    def footer(times: list[Time]):
        compilations = [time.compilation for time in times]
        executions = [time.execution for time in times]

        return (
            f"{statistics.mean(compilations) :.6f} ± {delta(compilations) :<10.6f} | " +
            f"{statistics.mean(executions) :.6f} ± {delta(executions) :<10.6f}"
        )

    return (
        "Compilation time (ms) | Execution time (ms)\n" +
        "----------------------+--------------------\n" +
        '\n'.join(map(line, times))              + '\n' +
        "----------------------+--------------------\n" +
        footer(times)
    )


def error(message: str):
    print(f"[Error] {message}", file = sys.stderr)
    sys.exit()


if __name__ == "__main__":
    main()
