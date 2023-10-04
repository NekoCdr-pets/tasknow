cmake_minimum_required(VERSION 3.27)
project(tasknow C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

add_executable(tasknow
    src/main.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(tasknow PRIVATE
        "-Weverything"
        "-Wno-c++98-compat"
        "-Wno-c++98-compat-pedantic"
        "-Wno-c++11-compat"
        "-Wno-c++11-compat-pedantic"
        "-Wno-c++14-compat"
        "-Wno-c++14-compat-pedantic"
        "-Wno-c++17-compat"
        "-Wno-c++17-compat-pedantic"
        "-Wno-c++20-compat"
        "-Wno-c++20-compat-pedantic"
        "-Wno-padded"
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(tasknow PRIVATE
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wconversion"
    )
endif()
