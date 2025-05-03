add_library(parser
    lexer.cpp
)
target_include_directories(parser
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/simpledb/parser
)
target_link_libraries(parser
    PUBLIC
)

add_executable(parser_unit_tests
    lexer.t.cpp
)
target_include_directories(parser_unit_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/simpledb/parser
)
target_link_libraries(parser_unit_tests
    PUBLIC
        parser
        Catch2::Catch2WithMain
)

