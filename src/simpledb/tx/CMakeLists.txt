add_library(tx
    transaction.cpp
)

target_include_directories(tx
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(tx
    PUBLIC
        file
        log
        buffer
)

add_executable(tx_test
    tx_test.cpp
)
target_include_directories(tx_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(tx_test
    PRIVATE
        server
)