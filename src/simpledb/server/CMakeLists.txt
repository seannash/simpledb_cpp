add_library(simpledb_server
    simpledb.cpp
)

target_include_directories(simpledb_server
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_server
    PUBLIC
        simpledb_tx
        simpledb_buffer
        simpledb_log
        simpledb_file
        simpledb_index
        simpledb_record
        simpledb_metadata
        simpledb_query
        simpledb_plan
)
