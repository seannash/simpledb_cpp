add_library(simpledb_tx
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

target_include_directories(simpledb_tx
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_tx
    PUBLIC
        simpledb_file
        simpledb_log
        simpledb_buffer
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
        simpledb_server
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
        simpledb_server
)