# CMake file for unit test executable

# List unit test source files
add_executable(${PROJECT_NAME}_tests
    # Tests
    translate_tests.cpp

    # Sources
)

# Add dependent libraries
target_link_libraries(${PROJECT_NAME}_tests
    gtest
    gtest_main
    gmock
    gmock_main
    pthread
)

# Add a commmand to build and run tests
add_custom_target(run_${PROJECT_NAME}_tests
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_tests
    DEPENDS ${PROJECT_NAME}_tests
)
