add_library(simpledb_index
    Index.cpp
    hash/HashIndex.cpp
    btree/BTPage.cpp
    btree/BTreeLeaf.cpp
    btree/BTreeDir.cpp
    btree/DirEntry.cpp
    btree/BTreeIndex.cpp
    query/IndexSelectScan.cpp
    planner/IndexSelectPlan.cpp
    planner/IndexJoinPlan.cpp
    query/IndexJoinScan.cpp
)

target_include_directories(simpledb_index
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_index
    PUBLIC
        simpledb_record
        simpledb_query
)

add_executable(index_retrieval_test
    IndexRetrievalTest.cpp
)

target_link_libraries(index_retrieval_test
    PRIVATE
        simpledb_server
)

add_executable(index_update_test
    IndexUpdateTest.cpp
)

target_link_libraries(index_update_test
    PRIVATE
        simpledb_server
)
