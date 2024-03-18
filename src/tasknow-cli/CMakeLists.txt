add_executable(tasknow-cli
    client.h
    client.cpp
    main.cpp
)
target_link_libraries(tasknow-cli PRIVATE
    Boost::program_options
    libtasknow
)
