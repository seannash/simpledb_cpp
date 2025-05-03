
add_library(query STATIC
    scan.cpp
    update_scan.cpp
    select_scan.cpp
    project_scan.cpp
    product_scan.cpp
    term.cpp
    constant.cpp
    expression.cpp
    predicate.cpp
)

target_include_directories(query
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(query
    PUBLIC
)

add_executable(query_unit_tests
    constant.t.cpp
)
target_include_directories(query_unit_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/simpledb/query
)
target_link_libraries(query_unit_tests
    PUBLIC
    query
    Catch2::Catch2WithMain
)

add_test(NAME query_unit_tests COMMAND query_unit_tests)