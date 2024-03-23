add_executable(tasknow-cli
    actions.h
    actions.cpp
    client.h
    client.cpp
    main.cpp
    options.h
    options.cpp
    response_handler.h
    response_handler.cpp
)
target_link_libraries(tasknow-cli PRIVATE
    Boost::program_options
    libtasknow
)
