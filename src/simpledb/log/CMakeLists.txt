add_library(log
    log_iterator.cpp
    log_manager.cpp
)

target_include_directories(log
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(log
    PUBLIC
        file
)

add_executable(log_test log_test.cpp)
target_include_directories(log_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(log_test
    PUBLIC
        log
        file
)