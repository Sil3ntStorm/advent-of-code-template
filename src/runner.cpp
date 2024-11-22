/**
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
**/

#include "runner.hpp"

#include "formatters.hpp"
#include "registrar.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>

namespace aoc {
Runner::Runner(int year, unsigned int day, const std::string& data_dir, const std::string& input_file_name) : m_year(year), m_day(day)
{
    read_puzzle(year, day, data_dir, input_file_name);
}

void Runner::solve(bool do_p1, bool do_p2)
{
    auto tm_st = std::chrono::steady_clock::now();
    try {
        auto solver        = registrar::get()->get_solver(m_year, m_day, m_puzzle);
        auto duration_load = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - tm_st);
        if (!solver) {
            std::cerr << "There is no solution for AoC " << m_year << " Day " << m_day << '\n';
            return;
        }
        std::println("[AoC {} Day {}] Loading and preprocessing input took {} | {}", m_year, m_day, human_time_diff(duration_load, 3), duration_load);
        if (do_p1) {
            auto start_p1 = std::chrono::steady_clock::now();
            try {
                auto r1          = solver->part1();
                auto duration_p1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_p1);
                std::println("[AoC {} Day {}] Part 1 solution is {}. Solved in {} | {}", m_year, m_day, r1, human_time_diff(duration_p1, 3), duration_p1);
            } catch (const std::exception& e) {
                auto ex_p1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_p1);
                std::cerr << "[AoC " << m_year << " Day " << m_day << "] Exception during part1: " << e.what() << " after " << human_time_diff(ex_p1, 3) << " | " << ex_p1 << '\n';
            }
        } else {
            std::println("[AoC {} Day {}] Part 1 SKIPPED", m_year, m_day);
        }
        if (do_p2) {
            auto start_p2 = std::chrono::steady_clock::now();
            try {
                auto r2          = solver->part2();
                auto duration_p2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_p2);
                std::println("[AoC {} Day {}] Part 2 solution is {}. Solved in {} | {}", m_year, m_day, r2, human_time_diff(duration_p2, 3), duration_p2);
            } catch (const std::exception& e) {
                auto ex_p2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_p2);
                std::cerr << "[AoC " << m_year << " Day " << m_day << "] Exception during part2: " << e.what() << " after " << human_time_diff(ex_p2, 3) << " | " << ex_p2 << '\n';
            }
        } else {
            std::println("[AoC {} Day {}] Part 2 SKIPPED", m_year, m_day);
        }
    } catch (const std::exception& e) {
        auto ex = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - tm_st);
        std::println("[AoC {} Day {}] Exception during preprocessing of input data: {} after {} | {}", m_year, m_day, e.what(), human_time_diff(ex, 3), ex);
    }
}

void Runner::read_puzzle(int year, unsigned int day, const std::string& data_dir, const std::string& file)
{
    std::filesystem::path path = std::format("{}/{}/{}/{}", data_dir, year, day, file);
    path                       = std::filesystem::absolute(path);
#ifdef _DEBUG
    if (!std::filesystem::exists(path)) {
        // When run from regular VS Solution Build
        path = std::format("../{}/{}/{}/{}", data_dir, year, day, file);
        if (!std::filesystem::exists(path)) {
            // When run from CMake build (including from within VS)
            path = std::format("../../{}/{}/{}/{}", data_dir, year, day, file);
        }
        path = std::filesystem::absolute(path);
    }
#endif
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(std::format("File {} not found", path.string()));
    }
    std::ifstream     ifs(path);
    std::stringstream buf;
    buf << ifs.rdbuf();
    m_puzzle = buf.str();
    if (m_puzzle.back() == '\n') {
        m_puzzle.pop_back();
    }
}
} // namespace aoc
