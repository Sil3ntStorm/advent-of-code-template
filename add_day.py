#! /usr/bin/env python3

"""
Copyright 2024 Sil3ntStorm https://github.com/sil3ntstorm

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import argparse
from datetime import datetime
import requests
from pathlib import Path
from pprintpp import pprint as pp

def inject(lines, after, to_add):
    try:
        idx = lines.index(after)
        idx = idx + 1
        return lines[:idx] + to_add + lines[idx:]
    except ValueError:
        pass
    return None


if __name__ == '__main__':
    cmdparse = argparse.ArgumentParser()
    cmdparse.add_argument('--year', type=int, choices=range(2015, datetime.now().year + 1))
    cmdparse.add_argument('--day', type=int, choices=range(1,25 + 1))
    cmdparse.add_argument('--token', type=str, required=True)
    params = cmdparse.parse_args()
    if (not params.year or not params.day) and (datetime.now().month != 12 or (datetime.now().month == 12 and datetime.now().day > 25)):
       raise Exception('Need to provide year and day parameters when AoC is not currently running')
    year = params.year or datetime.now().year
    day = params.day or datetime.now().day
    print(f'Preparing AoC {year}-{day:02}')

    root_dir = Path(__file__).resolve().parent

    # download input
    cookies = {'session': params.token}
    r = requests.get(f'https://adventofcode.com/{year}/day/{day}/input', cookies=cookies)
    if r.status_code != 200:
        raise Exception(f'Failed to download puzzle input: {r.status_code}')

    input_file = Path(root_dir, 'data', str(year), str(day), 'input.txt')
    input_file.parent.mkdir(parents=True, exist_ok=True)
    with open(input_file, 'w', newline='\n') as f:
        f.write(r.text)

    # Copy template
    template_dir = Path(root_dir, 'template')
    source_dir = Path(root_dir, 'puzzles', str(year))
    source_dir.mkdir(parents=True, exist_ok=True)
    hpp = ''
    cpp = ''

    with open(Path(template_dir, 'day_00.hpp'), 'r') as f:
        hpp = f.read()
    with open(Path(template_dir, 'day_00.cpp'), 'r') as f:
        cpp = f.read()
    with open(Path(source_dir, f'{year}_{day:02}.hpp'), 'w', newline='\n') as f:
        f.write(hpp.replace('$year$', str(year)).replace('$day$', str(day)))
    with open(Path(source_dir, f'{year}_{day:02}.cpp'), 'w', newline='\n') as f:
        f.write(cpp.replace('$year$', str(year)).replace('$day$', str(day)).replace('day_00.hpp', f'{year}_{day:02}.hpp'))

    # Update CMake File
    cmake_file = Path(root_dir, 'CMakeLists.txt')
    cmake_content = []
    with open(cmake_file, 'r') as f:
        cmake_content = [l.rstrip() for l in f.readlines()]
    search = f'# Puzzle Sources {year}'
    to_add = [f'    puzzles/{year}/{year}_{day:02}.hpp', f'    puzzles/{year}/{year}_{day:02}.cpp']
    try:
        idx = cmake_content.index(search)
        idx = idx + 2
        head = cmake_content[:idx]
        cmake_content = cmake_content[idx:]

        skip_days = 1
        while True:
            inject_after = f'    puzzles/{year}/{year}_{day - skip_days:02}.cpp'
            n = inject(cmake_content, inject_after, to_add)
            if n is not None:
                cmake_content = head + n
                break
            skip_days = skip_days + 1
            if skip_days >= day:
                cmake_content = head + to_add + cmake_content
                break
    except ValueError:
        # Year does not exist yet
        grp_name = f'aoc_{year}_files'
        to_add = [search, f'set({grp_name}'] + to_add + [')', 'target_sources(${PROJ_BIN_NAME} PRIVATE ${' + grp_name + '})', f'source_group(puzzles/{year} FILES $' + '{' + grp_name + '})' ,'']
        idx = cmake_content.index('# === Installation ===')
        head = cmake_content[:idx]
        cmake_content = head + to_add + cmake_content[idx:]
    if cmake_content[-1] != '':
        cmake_content.append('')
    with open(cmake_file, 'w', newline='\n') as f:
        f.write('\n'.join(cmake_content))
