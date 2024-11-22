#include "day_00.hpp"

#include "registrar.hpp"

#include <stdexcept>

namespace aoc::year_$year$::day_$day$ {

auto solver_id = aoc::registrar::get()->register_solver<aoc::year_$year$::day_$day$::Solver>($year$, $day$);

Solver::Solver(const std::string_view& puzzle) : base::Solver(puzzle)
{}

std::string Solver::part1()
{
    throw std::runtime_error("not implemented");
}

std::string Solver::part2()
{
    throw std::runtime_error("not implemented");
}

} // namespace aoc::year_$year$::day_$day$
