add_library(tx
    transaction.cpp
    bufferlist.cpp
    recovery/recovery_manager.cpp
    recovery/log_record.cpp
    recovery/start_log_record.cpp
    recovery/checkpoint_log_record.cpp
    recovery/commit_log_record.cpp
    recovery/rollback_log_record.cpp
    recovery/set_int_log_record.cpp
    recovery/set_string_log_record.cpp
    recovery/log_record_builder.cpp
    concurrency/lock_table.cpp
    concurrency/concurrency_manager.cpp
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


add_executable(tx_test_small
    tx_test_small.cpp
)
target_include_directories(tx_test_small
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(tx_test_small
    PRIVATE
        server
)