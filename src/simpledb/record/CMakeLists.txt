add_library(record
    schema.cpp
    layout.cpp
)

target_include_directories(record
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(record
    PUBLIC
        
)
