include(FetchContent)

FetchContent_Declare(
    boost-ut-pckg
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG cd12498349362cc646a7140451bf51db2a2dac00 #v1.1.9
)
FetchContent_Populate(boost-ut-pckg)
FetchContent_GetProperties(boost-ut-pckg SOURCE_DIR boost_ut_source_dir)

add_library(boost-ut INTERFACE IMPORTED GLOBAL)
add_library(Boost::ut ALIAS boost-ut)
target_include_directories(boost-ut SYSTEM INTERFACE
    ${boost_ut_source_dir}/include
)

add_executable(tasknow-test
    test.cpp
)
target_link_libraries(tasknow-test PRIVATE
    Boost::ut
)
