cmake_minimum_required(VERSION 3.21)

project(
        CxxLazy
        VERSION 0.1.0
        DESCRIPTION "Thread-safe lazy initialization for modern C++ | 现代 C++ 的线程安全惰性初始化"
        HOMEPAGE_URL "https://github.com/uyplayer/CxxLazy"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "🛠️ 检测到编译器: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wno-unused-parameter
    )
    message(STATUS "⚙️ 启用 GCC/Clang 编译选项: -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter")

elseif (MSVC)
    add_compile_options(/W4 /WX)
    message(STATUS "⚙️ MSVC 编译选项启用: /W4 /WX")
endif()


message(STATUS ${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake OPTIONAL)

message(STATUS "项目根目录 : ${CMAKE_SOURCE_DIR}")
message(STATUS "当前目录 : ${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")

enable_testing()
find_package(Catch2 REQUIRED)


add_subdirectory(tests)
add_subdirectory(src)