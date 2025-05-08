add_library(simpledb_multibuffer
    BufferNeeds.cpp
    ChunkScan.cpp
    MultibufferProductScan.cpp
    MultibufferProductPlan.cpp
)

target_include_directories(simpledb_multibuffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_multibuffer
    PUBLIC
        simpledb_plan
        simpledb_query
        simpledb_record
        simpledb_tx
)
