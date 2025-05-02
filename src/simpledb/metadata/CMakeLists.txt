add_library(metadata
    table_manager.cpp
    view_manager.cpp
    stat_manager.cpp
    stat_info.cpp
    index_info.cpp
    index_manager.cpp
)

target_include_directories(metadata
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(metadata
    PUBLIC
        record
        tx
)

add_executable(table_manager_test
    table_manager_test.cpp
)
target_include_directories(table_manager_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(table_manager_test
    PRIVATE
        metadata
        server
)

add_executable(catalog_test
    catalog_test.cpp
)
target_include_directories(catalog_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(catalog_test
    PRIVATE
        server
)

add_executable(metadata_manager_test
    metadata_manager_test.cpp
)
target_include_directories(metadata_manager_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(metadata_manager_test
    PRIVATE
        server
)
