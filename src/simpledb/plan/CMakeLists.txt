add_library(plan
    plan.cpp
    table_plan.cpp
    select_plan.cpp
)

target_include_directories(plan
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(plan
    PUBLIC
        query
        record
        metadata
        tx
)
