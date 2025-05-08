add_library(simpledb_materialize
    TempTable.cpp
    MaterializationPlan.cpp
    RecordComparator.cpp
    SortPlan.cpp
    SortScan.cpp
)

target_include_directories(simpledb_materialize
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_materialize
    PUBLIC
        simpledb_record
        simpledb_tx
        simpledb_plan
        simpledb_query
)
