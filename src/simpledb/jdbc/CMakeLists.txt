add_library(simpledb_jdbc STATIC
    driver_adapter.cpp
    connection_adapter.cpp
    statement_adapter.cpp
    result_set_adapter.cpp
    embedded/embedded_driver.cpp
    embedded/embedded_connection.cpp
    embedded/embedded_statement.cpp
    embedded/embedded_result_set.cpp
    metadata_adapter.cpp
)
target_include_directories(simpledb_jdbc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(simpledb_jdbc
    PUBLIC
        jdbc
        server
)

add_executable(embedded_test embedded/embeded_test.cpp)
target_include_directories(embedded_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(embedded_test
    PUBLIC
        simpledb_jdbc
)