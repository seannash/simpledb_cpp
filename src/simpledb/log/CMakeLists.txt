add_library(simpledb_log
    LogIterator.cpp
    LogMgr.cpp
)

target_include_directories(simpledb_log
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_log
    PUBLIC
        simpledb_file
)

add_executable(log_test LogTest.cpp)
target_include_directories(log_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(log_test
    PUBLIC
        simpledb_log
        simpledb_file
)