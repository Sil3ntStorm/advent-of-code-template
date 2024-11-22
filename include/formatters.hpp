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

#include <chrono>
#include <iosfwd>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 202207L
#include <format>
#include <vector>

template<typename T>
requires std::is_integral_v<T>
struct std::formatter<std::vector<T>> : std::formatter<std::string_view>
{
    auto format(const std::vector<T>& input, std::format_context& ctx) const noexcept
    {
        std::string s{};
        for (auto&& val : input) {
            if (!s.empty()) {
                s += ", ";
            }
            s += std::format("{}", val);
        }
        return std::formatter<std::string_view>::format(std::format("[{}]", s), ctx);
    }
};
#endif

std::string human_time_diff(std::chrono::nanoseconds duration, std::streamsize precision);
