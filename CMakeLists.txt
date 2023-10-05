cmake_minimum_required(VERSION 3.27)
project(tasknow C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

add_library(libtasknow OBJECT
    src/buffer.h
    src/defines.h
    src/task.h
    src/task.cpp
)
target_include_directories(libtasknow PUBLIC src)

add_executable(tasknowd
    src/main.cpp
)
target_link_libraries(tasknowd PRIVATE libtasknow)

add_subdirectory(tests)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(libtasknow PUBLIC
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
        "-Wno-unsafe-buffer-usage" # https://github.com/llvm/llvm-project/issues/64646#issuecomment-1747952699
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(libtasknow PUBLIC
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wconversion"
    )
endif()
