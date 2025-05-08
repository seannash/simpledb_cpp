# Create file system library
add_library(simpledb_file
    BlockId.cpp
    BufferWrapper.cpp
    FileMgr.cpp  
    Page.cpp
)

# Set include directories
target_include_directories(simpledb_file
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set properties
set_target_properties(simpledb_file PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "file"
)

add_executable(page_test Page.t.cpp)
target_link_libraries(page_test PRIVATE simpledb_file Catch2::Catch2WithMain)

add_executable(block_id_test BlockId.t.cpp)
target_link_libraries(block_id_test PRIVATE simpledb_file Catch2::Catch2WithMain)

add_executable(file_manager_test FileMgr.t.cpp)
target_link_libraries(file_manager_test PRIVATE simpledb_file Catch2::Catch2WithMain)

# Register test with CTest
include(CTest)
add_test(NAME block_id_test COMMAND block_id_test)
add_test(NAME page_test COMMAND page_test)
add_test(NAME file_manager_test COMMAND file_manager_test)
