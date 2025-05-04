add_library(jdbc STATIC
    driver.cpp
    connection.cpp
    statement.cpp
    result_set.cpp
    sql_exception.cpp
    metadata.cpp
)

target_include_directories(jdbc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

