#!/usr/bin/python
from typing import Iterable, Any, TypeVar, Callable, Hashable
import os
from rich.console import Console
from itertools import chain
import subprocess

Flags = tuple[str, ...]

ROOT = os.getcwd()

def rel(*paths: str) -> str:
    return os.path.join(ROOT, *paths)


BUILD = rel("build")
SOURCE = rel("src")

CC = "gcc"
DEFAULT_CXFLAGS = ("-Wall", "-Wextra", "-O2", f"-I{rel(SOURCE, 'test')}", "-c")
DEFAULT_LDFLAGS = ()

console = Console()

def system_with_output(cmd: str) -> str:
    proc = subprocess.Popen([cmd], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    return out.decode()

def system(cmd: str):
    # console.log("[yellow]" + cmd)
    os.system(cmd)

def clear():
    system(f"rm -rf {rel(BUILD)}")
    system(f"mkdir -p {rel(BUILD, 'test')}")

def ld(target: str, files: list[str], *, addition_ldflags: Flags = ()):
    ldflags = DEFAULT_LDFLAGS + addition_ldflags
    ldflag_str = " ".join(ldflags)
    files_str = " ".join(files)

    system(f"{CC} {ldflag_str} -o {target} {files_str}")


def cc(src_path: str, obj_path: str, *, addition_cxflags: Flags = (), addition_ldflags: Flags = ()):
    cxflags = DEFAULT_CXFLAGS + addition_cxflags
    ldflags = DEFAULT_LDFLAGS + addition_ldflags

    cxflag_str = " ".join(cxflags)
    ldflag_str = " ".join(ldflags)

    system(f"{CC} {cxflag_str} {ldflag_str} -o {obj_path} {src_path}")



def ls_full(path: str) -> Iterable[str]:
    for file in os.listdir(path):
        yield os.path.join(path, file)


def build_test():
    for filename in os.listdir(rel(SOURCE, "test")):
        if filename.endswith(".c"):
            src_full = rel(SOURCE, "test", filename)
            obj_full = rel(BUILD, "test", filename.replace(".c", ".o"))
            cc(src_full, obj_full)


# type: ignore
def simple_build(target: str, *, addition_cxflags: Flags = (), addition_ldflags: Flags = ()):
    build_test()

    target_src = rel(SOURCE, target + ".c")
    target_obj = rel(BUILD, target + ".o")
    target_bin = rel(BUILD, target)

    cc(target_src, target_obj,
        addition_cxflags=addition_cxflags, addition_ldflags=addition_ldflags)

    ld(target_bin, [target_obj, *ls_full(rel(BUILD, "test"))],
        addition_ldflags=addition_ldflags)


config = {
    "00_baseline": lambda: simple_build("00_baseline"),
    "01_adjust_loop_order": lambda: simple_build("01_adjust_loop_order"),
    "02_multi_thread": lambda: simple_build("02_multi_thread", addition_ldflags=("-pthread",)),
    "03_omp": lambda: simple_build("03_omp", addition_cxflags=("-fopenmp",), addition_ldflags=("-fopenmp",)),
    "04_mpi": lambda: simple_build("04_mpi", addition_ldflags=("-I/usr/include/openmpi-x86_64", "-L/usr/lib64/openmpi/lib", "-Wl,-rpath", "-Wl,/usr/lib64/openmpi/lib", "-Wl,--enable-new-dtags", "-lmpi"))
}

def subcommand_build(argv: list[str]):
    target_prefix = argv[0]

    target_coord = [s for s in os.listdir(SOURCE) if s.startswith(target_prefix)]
    
    if len(target_coord) != 1:
        print("Multi or no target(s) available: ", target_coord)
        exit(1)

    target = target_coord[0].removesuffix(".c")

    if target not in config:
        print("target not in config: ", target)
        exit(1)

    config[target]()

T = TypeVar('T')
U = TypeVar('U', bound=Hashable)
def unique(it: Iterable[T], key: Callable[[T], U]) -> Iterable[T]:
    keys = set()
    for e in it:
        if (k := key(e)) not in keys:
            keys.add(k)
            yield e

def generate_all_compile_commands():
    import json
    DB_NAME = 'compile_commands.json'

    def generate_one(target: str) -> list[dict[str, Any]]:
        system(f"bear -- ./m build {target}")
        with open(rel(DB_NAME), 'r') as f:
            return json.load(f)

    entries = unique(chain.from_iterable(map(generate_one, config.keys())), key=lambda x: x['file'])
    with open(rel(BUILD, DB_NAME), 'w') as f:
        json.dump(list(entries), f)

    if os.path.exists(rel(DB_NAME)):
        os.remove(rel("compile_commands.json"))

def test_all():
    for config_name in config:
        subcommand_build([config_name])

        for size in [256, 512, 1024]:
            result = system_with_output(f"build/{config_name} {size}")
            print(f"Test {config_name}({size}): {result}")
            


if __name__ == '__main__':
    import sys
    _, subcommand, *rest = sys.argv

    match (subcommand):
        case "build": subcommand_build(rest)
        case "clear": clear()
        case "export-compile-commands": generate_all_compile_commands()
        case "test-all": test_all() 
