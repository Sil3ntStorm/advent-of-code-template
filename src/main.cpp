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

#include "registrar.hpp"
#include "runner.hpp"

#include <CLI/CLI.hpp>

#include <charconv>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

template<typename T>
requires std::is_integral_v<T>
[[nodiscard]] std::optional<T> convert_to(const std::string_view& sv)
{
    if (sv.empty()) {
        return std::nullopt;
    }
    T   out{};
    int off = 0;
    if (sv.front() == '+') {
        off = 1;
    }
    if constexpr (std::is_unsigned_v<T>) {
        if (sv.front() == '-') {
            return std::nullopt;
        }
    }
    auto conv_res = std::from_chars(sv.data() + off, sv.data() + sv.size(), out);
    if (conv_res.ec == std::errc::invalid_argument || conv_res.ec == std::errc::result_out_of_range) {
        return std::nullopt;
    }
    return out;
}

int main(int argc, char** argv)
{
    int          year{0};
    unsigned int day{0};
    std::string  file{"input.txt"};
    bool         skip1 = false;
    bool         skip2 = false;
    std::string  data_dir{"data"};
    bool         use_latest = false;
    auto         now        = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
    auto         max_year   = static_cast<int>(now.year());

    if (now.month() != std::chrono::December) {
        --max_year;
    }
    constexpr auto k_min_year = 2015;
    constexpr auto k_max_day  = 25;

    CLI::App cli("Advent of Code Helper", "Advent of Code");
    cli.add_option("--year,-y", year, "Which year of Advent of Code to run")->check(CLI::Range(k_min_year, max_year));
    cli.add_option("--day,-d", day, "The day of the year to run")->check(CLI::Range(1, k_max_day));
    cli.add_option("--file,-f", file, "Which demo file to load")->default_str("input.txt");
    cli.add_flag("--skip-part1", skip1, "Skip part 1 of the solution running (for long running solutions)");
    cli.add_flag("--skip-part2", skip2, "Skip part 2 of the solution running (for long running solutions)");
    cli.add_option("--data-dir", data_dir, "Root Directory for puzzle input data")->check(CLI::ExistingDirectory);
    cli.add_flag("--latest", use_latest, "Run the latest available solution (optionally of a given year)");
    try {
        cli.parse(argc, argv);
    } catch (const CLI::CallForVersion&) {
        // Needs to catch before success (base).
        // Weird thing of CLI11, which throws this and makes you do shit
        // instead of doing it itself.
        // User requested version info
        std::cout << cli.version() << '\n';
        return 0;
    } catch (const CLI::Success&) {
        // Needs to catch and not log an error, weird thing of CLI11
        // Thrown for help, version and the like.
        std::cout << cli.help() << '\n';
        return 0;
    } catch (const CLI::ParseError& e) {
        std::cerr << e.what() << '\n';
        std::cout << cli.help() << '\n';
        return -1;
    }
    if (use_latest) {
        if (year == 0) {
            year = aoc::registrar::get()->get_latest_year();
        }
        while (year >= k_min_year) {
            day = aoc::registrar::get()->get_latest_day_for_year(year);
            if (day != 0) {
                break;
            }
            --year;
        }
    }
    if (year == 0 && day == 0 && now.month() != std::chrono::December) {
        std::cout << "Specify Year to run [" << max_year << "]: ";
        std::string inp;
        std::getline(std::cin, inp);
        if (inp.empty()) {
            year = max_year;
        } else {
            year = convert_to<int>(inp).value_or(0);
        }
        inp.clear();
        auto today = static_cast<uint32_t>(now.day());
        if (today > k_max_day) {
            today = k_max_day;
        }
        std::cout << "Specify day to run [" << today << "]: ";
        std::getline(std::cin, inp);
        if (inp.empty()) {
            day = today;
        } else {
            day = convert_to<uint32_t>(inp).value_or(0);
        }
    }
    if (year <= 0 || year > max_year) {
        year = max_year;
    }
    if (day <= 0 || day > k_max_day) {
        day = static_cast<unsigned int>(now.day());
        if (day > k_max_day) {
            day = k_max_day;
        }
    }
    try {
        aoc::Runner aoc(year, day, data_dir, file);
        aoc.solve(!skip1, !skip2);
    } catch (const std::runtime_error& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return -2;
    }
    return 0;
}
