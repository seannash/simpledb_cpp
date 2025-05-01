add_library(buffer
    buffer.cpp
    buffer_manager.cpp
)

target_include_directories(buffer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(buffer
    PUBLIC
        file
        log
)

# Add test executable
add_executable(buffer_test
    buffer_test.cpp
)
target_include_directories(buffer_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(buffer_test
    PRIVATE
        buffer
)