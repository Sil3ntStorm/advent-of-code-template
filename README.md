# Advent of Code C++ Template

This is a C++ skeleton project for [Advent of Code](https://adventofcode.com) made by [Sil3ntStorm](https://github.com/sil3ntstorm).

## Requirements
This project makes use of C++23 features, so a compiler with C++23 support is required.

The [CLI11](https://github.com/CLIUtils/CLI11) library is required for the command line interface.

A `conanfile.py` is provided if you choose to manage your dependencies with [Conan](https://conan.io), however you can also manage your dependencies however you see fit.

## Usage

It will create a single application that can run the puzzle for a given `--year` and `--day`, or alternatively the `--latest` puzzle to which you created a solution. If a year is specified it runs the latest solution available for the provided year.  
The puzzle input is read automatically and available in the `m_puzzle` member of the `Solver` class.  
The directory form which inputs are read (default `data/{year}/{day}`) can be specified using `--data-dir`  
The filename to read (default `input.txt`) can be specified using `--file` (e.g. for when you want to debug using the example input)  
If desired you can skip running part1 or part2 of the daily puzzle using `--skip-part1` and `--skip-part2`

## Adding a days puzzle and download the input
#### Getting the session token
- Log in to the [Advent of Code](https://adventofcode.com) website
- Get the value of the session token from the cookies (ie. using Browser dev tools).

You can download the puzzle input for a given day using `add_day.py --session {YOUR_SESSION_TOKEN}` which will download and prepare the files for today (if Advent of Code is currently running).  
If you want to add a puzzle from the past you can do so by adding the `--year` and `--day` parameters.

The script will automatically create a basic .cpp and .hpp file for implementing the solution and add it to the `CMakeLists.txt` for your convenience.  
You can modify the template created (ie. if you would like to have additional boilerplate or includes available) in the `templates/` directory.

The puzzle input will be downloaded into the `data/{year}/{day}` folder, which will be created if they don't exist. :warn: This directory is **intentionally** not included in the repository as (per AoC rules) you shall not publish your puzzle input or include it in repositories.

The implementation files are created in the `puzzles/{year}` directory, which will be created if it does not exist.
