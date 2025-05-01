# Create JDBC library
add_library(jdbc
    jdbc.cpp
    sql_exception.cpp
    properties.cpp
    mock_driver.cpp
    driver_manager.cpp
    connection.cpp
    prepared_statement.cpp
    result_set.cpp
    statement.cpp
    driver.cpp
    fake_internal_connection.cpp
    result_set_metadata.cpp
    in_memory_result_set.cpp

)

# Set include directories
target_include_directories(jdbc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set properties
set_target_properties(jdbc PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "jdbc"
)

# Fetch and configure Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(jdbc_properties_test properties.t.cpp)
target_link_libraries(jdbc_properties_test PRIVATE jdbc Catch2::Catch2WithMain)

add_executable(jdbc_test jdbc.t.cpp)
target_link_libraries(jdbc_test PRIVATE jdbc Catch2::Catch2WithMain)


# Register test with CTest
include(CTest)
add_test(NAME jdbc_properties_test COMMAND jdbc_properties_test)
add_test(NAME jdbc_test COMMAND jdbc_test)