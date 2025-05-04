
add_subdirectory(embedded)

add_executable(student_major_no_jdbc
    StudentMajorNoJDBC.cpp
)
target_link_libraries(student_major_no_jdbc
    simpledb
)
target_include_directories(student_major_no_jdbc PRIVATE
    simpledb
)

add_executable(simple_ij
    SimpleIj.cpp
)
target_link_libraries(simple_ij
    simpledb
)
target_include_directories(simple_ij PRIVATE
    simpledb
)

