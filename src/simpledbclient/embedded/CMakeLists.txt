add_executable(change_major
    ChangeMajor.cpp
)
target_link_libraries(change_major
    simpledb
)
target_include_directories(change_major PRIVATE
    simpledb
)

add_executable(create_student_db
    CreateStudentDB.cpp
)
target_link_libraries(create_student_db
    simpledb
)
target_include_directories(create_student_db PRIVATE
    simpledb
)

add_executable(find_majors
    FindMajors.cpp
)
target_link_libraries(find_majors
    simpledb
)
target_include_directories(find_majors PRIVATE
    simpledb
)

add_executable(student_major
    StudentMajor.cpp
)
target_link_libraries(student_major
    simpledb
)
target_include_directories(student_major PRIVATE
    simpledb
)

add_executable(create_student_db_index
    CreateStudentDB-index.cpp
)
target_link_libraries(create_student_db_index
    simpledb
)