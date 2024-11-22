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

namespace aoc {
std::shared_ptr<registrar> aoc::registrar::get()
{
    static std::shared_ptr<registrar> inst{std::make_shared<registrar>()};
    return inst;
}

Solver_Instance registrar::get_solver(int year, unsigned int day, const std::string_view& puzzle)
{
    auto key = map_key(year, day);
    if (m_active_solvers.contains(key)) {
        return m_active_solvers.at(key);
    }
    if (m_solvers.contains(key)) {
        auto&& maker = m_solvers.at(key);
        return m_active_solvers.emplace(key, maker(puzzle)).first->second;
    }
    return nullptr;
}

int registrar::get_latest_year()
{
    int year{};
    for (auto&& s : m_solvers) {
        year = std::max(year, static_cast<int>(s.first >> 16));
    }
    return year;
}

unsigned int registrar::get_latest_day_for_year(int year)
{
    int64_t      y = static_cast<int64_t>(year) << 16;
    unsigned int day{};
    for (auto&& s : m_solvers) {
        if ((s.first & y) == y) {
            day = std::max(day, static_cast<unsigned int>(s.first & 0xFFFF));
        }
    }
    return day;
}

} // namespace aoc
