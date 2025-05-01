# Create file system library
add_library(file
    page.cpp
    file_manager.cpp  
    block_id.cpp
)

# Set include directories
target_include_directories(file
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set properties
set_target_properties(file PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "file"
)

# Enable C++20 features
target_compile_features(file PRIVATE cxx_std_20)

# Add test executables
add_executable(page_test page.t.cpp)
target_link_libraries(page_test PRIVATE file Catch2::Catch2WithMain)

add_executable(block_id_test block_id.t.cpp)
target_link_libraries(block_id_test PRIVATE file Catch2::Catch2WithMain)

add_executable(file_manager_test file_manager.t.cpp)
target_link_libraries(file_manager_test PRIVATE file Catch2::Catch2WithMain)

# Register test with CTest
include(CTest)
add_test(NAME block_id_test COMMAND block_id_test)
add_test(NAME page_test COMMAND page_test)
add_test(NAME file_manager_test COMMAND file_manager_test)
