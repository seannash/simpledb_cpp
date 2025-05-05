add_library(simpledb_index
    Index.hpp
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

##[[
add_executable(index_retrieval_test
    IndexRetrievalTest.cpp
)

target_link_libraries(index_retrieval_test
    PRIVATE
        simpledb_server
)
#]]