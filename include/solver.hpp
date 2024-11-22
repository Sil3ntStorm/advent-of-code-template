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
#include <string>
#include <string_view>

namespace aoc::base {
class Solver
{
  public:
    Solver(const std::string_view& puzzle);
    virtual ~Solver();

    virtual std::string part1() = 0;
    virtual std::string part2() = 0;

  protected:
    std::string_view m_puzzle;
};
} // namespace aoc::base
