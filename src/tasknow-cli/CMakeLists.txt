add_executable(tasknow-cli
    client.h
    client.cpp
    main.cpp
    response_handler.h
    response_handler.cpp
)
target_link_libraries(tasknow-cli PRIVATE
    Boost::program_options
    libtasknow
)
