from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.microsoft import MSBuildDeps
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps, CMake

class AoC_Conan(ConanFile):
    name = "advent-of-code"
    version = "0.1"
    settings = "os", "arch", "compiler", "build_type"
    topics = ("aoc")
    languages = "C++"
    package_type = "application"

    default_options = {
        "spdlog/*:header_only": True,
        "spdlog/*:use_std_fmt": True,
    }

    def requirements(self):
        self.requires("cli11/2.4.2")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        ms = MSBuildDeps(self)
        ms.generate()
        tc = CMakeToolchain(self)
        tc.generate()
        cd = CMakeDeps(self)
        cd.generate()
