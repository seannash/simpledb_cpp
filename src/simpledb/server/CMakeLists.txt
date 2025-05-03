add_library(server
    simpledb.cpp
)

target_include_directories(server
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(server
    PUBLIC
        tx
        buffer
        log
        file
        record
        metadata
        query
)
