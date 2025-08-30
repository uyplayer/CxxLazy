from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class CxxLazyRecipe(ConanFile):
    name = "cxxlazy"
    version = "0.1"

    settings = "os", "compiler", "build_type", "arch"

    requires = ("catch2/3.5.2")
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["cxxlazy"]