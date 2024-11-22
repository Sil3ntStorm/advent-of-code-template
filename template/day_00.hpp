#pragma once
#include "solver.hpp"
#include <string_view>

namespace aoc::year_$year$::day_$day$
{
class Solver : public base::Solver
{
  public:
    Solver(const std::string_view& puzzle);

    std::string part1() override;
    std::string part2() override;
};
} // namespace aoc::year_$year$::day_$day$
