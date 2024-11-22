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

namespace aoc {
class Runner
{
  public:
    Runner(int year, unsigned int day, const std::string& data_dir, const std::string& input_file_name);

    void solve(bool run1, bool run2);

  private:
    void read_puzzle(int year, unsigned int day, const std::string& data_dir, const std::string& file = "input.txt");

    std::string  m_puzzle;
    int          m_year;
    unsigned int m_day;
};
} // namespace aoc
