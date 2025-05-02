add_library(record
    schema.cpp
    layout.cpp
    record_page.cpp
)

target_include_directories(record
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(record
    PUBLIC 
        tx 
        file
)

add_executable(record_test
    record_test.cpp
)
target_include_directories(record_test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(record_test
    server
)