add_subdirectory(file) 
add_subdirectory(log) 
add_subdirectory(buffer) 
add_subdirectory(tx) 
add_subdirectory(record)
add_subdirectory(metadata)
add_subdirectory(query)
add_subdirectory(parse)
add_subdirectory(plan)
add_subdirectory(index)
add_subdirectory(materialize)
add_subdirectory(multibuffer)
add_subdirectory(server)
add_subdirectory(jdbc) 

add_library(simpledb STATIC
    $<TARGET_OBJECTS:jdbc>
    $<TARGET_OBJECTS:simpledb_file>
    $<TARGET_OBJECTS:simpledb_index>
    $<TARGET_OBJECTS:simpledb_log>
    $<TARGET_OBJECTS:simpledb_buffer>
    $<TARGET_OBJECTS:simpledb_tx>
    $<TARGET_OBJECTS:simpledb_record>
    $<TARGET_OBJECTS:simpledb_metadata>
    $<TARGET_OBJECTS:simpledb_query>
    $<TARGET_OBJECTS:simpledb_parser>
    $<TARGET_OBJECTS:simpledb_plan>
    $<TARGET_OBJECTS:simpledb_server>
    $<TARGET_OBJECTS:simpledb_jdbc>
    $<TARGET_OBJECTS:simpledb_materialize>
    $<TARGET_OBJECTS:simpledb_multibuffer>
)

target_include_directories(simpledb
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)


