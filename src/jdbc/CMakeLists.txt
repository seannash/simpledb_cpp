add_library(jdbc STATIC
    Driver.cpp
    Connection.cpp
    Metadata.cpp
    ResultSet.cpp
    Statement.cpp
    SQLException.cpp
)

target_include_directories(jdbc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

