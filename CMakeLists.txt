cmake_minimum_required(VERSION 3.23)

project(AoC VERSION 1.0.0 LANGUAGES CXX HOMEPAGE_URL https://github.com/sil3ntstorm/advent-of-code-template)
set(PROJ_BIN_NAME "advent-of-code")
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CXX_STANDARD_REQUIRED ON)
if(NOT "${CMAKE_CXX_STANDARD}" OR CMAKE_CXX_STANDARD LESS 23)
    set(CMAKE_CXX_STANDARD 23)
endif()

add_executable(${PROJ_BIN_NAME})

target_include_directories(${PROJ_BIN_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)

find_package(spdlog REQUIRED)
find_package(cli11 REQUIRED)
find_package(Microsoft.GSL REQUIRED)

target_link_libraries(${PROJ_BIN_NAME} PRIVATE spdlog::spdlog_header_only)
target_link_libraries(${PROJ_BIN_NAME} PRIVATE CLI11::CLI11)
target_link_libraries(${PROJ_BIN_NAME} PRIVATE Microsoft.GSL::GSL)

# Make spdlog use std::format instead of using fmtlib
target_compile_definitions(${PROJ_BIN_NAME} PUBLIC SPDLOG_USE_STD_FORMAT)
# Enable Weak algorithms (e.g. MD5)
target_compile_definitions(${PROJ_BIN_NAME} PUBLIC CRYPTOPP_ENABLE_NAMESPACE_WEAK)

# Because CMake refuses to set these flags for MSVC
if(MSVC AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${PROJ_BIN_NAME} PUBLIC "/Zc:__cplusplus")
    target_compile_options(${PROJ_BIN_NAME} PUBLIC "/Zc:enumTypes")
    target_compile_options(${PROJ_BIN_NAME} PUBLIC "/Zc:templateScope")
    target_compile_options(${PROJ_BIN_NAME} PUBLIC "/permissive-")
endif()
if(MSVC AND (MSVC_VERSION GREATER_EQUAL 1934))
    target_compile_options(${PROJ_BIN_NAME} PRIVATE /external:anglebrackets /external:W2)
endif()

# Set proper warning options
if(MSVC)
    target_compile_options(${PROJ_BIN_NAME} PRIVATE /W4 /WX /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928)
else()
    target_compile_options(${PROJ_BIN_NAME} PRIVATE -Werror -Weverything -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wlifetime -Wimplicit-fallthrough -Weffc++)
endif()

# Actual source files
target_sources(${PROJ_BIN_NAME} PRIVATE
    src/formatters.cpp
    src/main.cpp
    src/registrar.cpp
    src/runner.cpp
    src/solver.cpp
)

# Public headers
target_sources(${PROJ_BIN_NAME} PUBLIC FILE_SET pub_headers TYPE HEADERS FILES
)

# Private headers
target_sources(${PROJ_BIN_NAME} PRIVATE
    include/formatters.hpp
    include/registrar.hpp
    include/runner.hpp
    include/solver.hpp
)

# === Installation ===
