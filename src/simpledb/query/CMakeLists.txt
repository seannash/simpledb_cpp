
add_library(query STATIC
    scan.cpp
    update_scan.cpp
    select_scan.cpp
    project_scan.cpp
    product_scan.cpp
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
