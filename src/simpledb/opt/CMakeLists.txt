add_library(simpledb_opt
    TablePlanner.cpp
    HeuristicQueryPlanner.cpp
)

target_include_directories(simpledb_opt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_opt
    PUBLIC
        simpledb_plan
        simpledb_query
        simpledb_record
        simpledb_tx
        simpledb_metadata
        simpledb_index
        simpledb_multibuffer
)
