add_library(simpledb_record
    Schema.cpp
    Layout.cpp
    RecordPage.cpp
    TableScan.cpp
    RID.cpp
)

target_include_directories(simpledb_record
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(simpledb_record
    PUBLIC 
        simpledb_tx 
        simpledb_file
)

add_executable(record_test
    RecordTest.cpp
)
target_include_directories(record_test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(record_test
    simpledb_server
)

add_executable(table_scan_test
    TableScanTest.cpp
)
target_include_directories(table_scan_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(table_scan_test
    simpledb_server
)