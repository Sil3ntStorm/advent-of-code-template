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

#pragma once
#include "solver.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace aoc {
using Solver_Instance = std::shared_ptr<base::Solver>;
using Solver_Creator  = std::function<Solver_Instance(const std::string_view&)>;
class registrar
{
  public:
    static std::shared_ptr<registrar> get();
    template<class T> auto            register_solver(int year, unsigned int day)
    {
        auto key = map_key(year, day);
        m_solvers.emplace(key, [](const std::string_view& puzzle) {
            return std::make_shared<T>(puzzle);
        });
        return key;
    };
    Solver_Instance get_solver(int year, unsigned int day, const std::string_view& puzzle);
    int             get_latest_year();
    unsigned int    get_latest_day_for_year(int year);

  private:
    std::unordered_map<int64_t, Solver_Creator>  m_solvers;
    std::unordered_map<int64_t, Solver_Instance> m_active_solvers;

    [[nodiscard]] constexpr int64_t map_key(int year, unsigned int day) const noexcept { return static_cast<int64_t>(year) << 16 | day; }
};

} // namespace aoc
