
cmake_minimum_required(VERSION 3.21)

project(
        CxxLazyTest
        VERSION 0.1.0
        DESCRIPTION "Thread-safe lazy initialization for modern C++ | 现代 C++ 的线程安全惰性初始化"
        HOMEPAGE_URL "https://github.com/uyplayer/CxxLazy"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(once_call)
add_subdirectory(lazy)
add_subdirectory(macros)
