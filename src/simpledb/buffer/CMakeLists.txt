add_library(simpledb_buffer
    Buffer.cpp
    BufferMgr.cpp
)

target_include_directories(simpledb_buffer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_buffer
    PUBLIC
        simpledb_file
        simpledb_log
)

# Add test executable
add_executable(buffer_test
    BufferTest.cpp
)
target_include_directories(buffer_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(buffer_test
    PRIVATE
        simpledb_buffer
)