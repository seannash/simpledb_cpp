add_library(simpledb_materialize
    TempTable.cpp
    MaterializePlan.cpp
    RecordComparator.cpp
    SortPlan.cpp
    SortScan.cpp
    MaxFn.cpp
    AggregateFn.cpp
    GroupByPlan.cpp
    GroupByScan.cpp
    GroupValue.cpp
    MergeJoinScan.cpp
    MergeJoinPlan.cpp
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
