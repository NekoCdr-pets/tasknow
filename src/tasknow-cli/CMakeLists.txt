add_executable(tasknow-cli
    main.cpp
)
target_link_libraries(tasknow-cli PRIVATE
    libtasknow
)