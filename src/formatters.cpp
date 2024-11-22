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

#include "formatters.hpp"

#include <chrono>
#include <optional>

std::string human_time_diff(std::chrono::nanoseconds duration, std::streamsize precision)
{
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    std::stringstream os;
    using namespace std::chrono;
    auto y = duration_cast<years>(duration);
    duration -= y;
    auto mn = duration_cast<months>(duration);
    duration -= mn;
    auto d = duration_cast<days>(duration);
    duration -= d;
    auto h = duration_cast<hours>(duration);
    duration -= h;
    auto m = duration_cast<minutes>(duration);
    duration -= m;
    auto s = duration_cast<seconds>(duration);
    duration -= s;

    std::optional<decltype(duration)::rep> fs_count;
    if (precision > os.precision()) {
        precision = os.precision();
    }
    if (precision > 6) {
        precision = 9;
    } else if (precision > 3) {
        precision = 6;
    } else if (precision > 0) {
        precision = 3;
    }
    switch (precision) {
        case 9:
            fs_count = duration.count();
            break;
        case 6:
            fs_count = duration_cast<microseconds>(duration).count();
            break;
        case 3:
            fs_count = duration_cast<milliseconds>(duration).count();
            break;
    }

    char fill = os.fill('0');
    if (y.count())
        os << y.count() << " year" << (y.count() > 1 ? "s" : "") << " ";
    if (mn.count())
        os << mn.count() << " month" << (mn.count() > 1 ? "s" : "") << " ";
    if (y.count() || mn.count() || d.count())
        os << d.count() << "d ";
    if (y.count() || mn.count() || d.count() || h.count())
        os << std::setw(2) << h.count() << ":";
    if (y.count() || mn.count() || d.count() || h.count() || m.count())
        os << std::setw(d.count() || h.count() ? 2 : 1) << m.count() << ":";
    os << std::setw(d.count() || h.count() || m.count() ? 2 : 1) << s.count();
    if (fs_count.has_value() && fs_count.value() > 0)
        os << "." << std::setw(precision) << fs_count.value();
    if (!d.count() && !h.count() && !m.count())
        os << "s";

    os.fill(fill);
    return os.str();
#else
    return {};
#endif
}
