add_library(simpledb_materialize
    TempTable.cpp
)

target_include_directories(simpledb_materialize
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_materialize
    PUBLIC
        simpledb_record
        simpledb_tx
)
