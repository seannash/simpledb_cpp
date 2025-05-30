#include "simpledb/jdbc/embedded/embedded_driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/statement.hpp"
#include "jdbc/sql_exception.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    auto d = std::make_unique<simpledb::jdbc::embedded::EmbeddedDriver>();
    std::string url = "studentdb";

    try {
        auto conn = d->connect(url, {});
        auto stmt = conn->createStatement();

        // Create and populate STUDENT table
        std::string s = "create table STUDENT(SId int, SName varchar(10), MajorId int, GradYear int)";
        stmt->executeUpdate(s);
        std::cout << "Table STUDENT created." << std::endl;

        s = "insert into STUDENT(SId, SName, MajorId, GradYear) values ";
        std::vector<std::string> studvals = {
            "(1, 'joe', 10, 2021)",
            "(2, 'amy', 20, 2020)",
            "(3, 'max', 10, 2022)",
            "(4, 'sue', 20, 2022)",
            "(5, 'bob', 30, 2020)",
            "(6, 'kim', 20, 2020)",
            "(7, 'art', 30, 2021)",
            "(8, 'pat', 20, 2019)",
            "(9, 'lee', 10, 2021)"
        };
        for (const auto& val : studvals) {
            stmt->executeUpdate(s + val);
        }
        std::cout << "STUDENT records inserted." << std::endl;

        // Create and populate DEPT table
        s = "create table DEPT(DId int, DName varchar(8))";
        stmt->executeUpdate(s);
        std::cout << "Table DEPT created." << std::endl;

        s = "insert into DEPT(DId, DName) values ";
        std::vector<std::string> deptvals = {
            "(10, 'compsci')",
            "(20, 'math')",
            "(30, 'drama')"
        };
        for (const auto& val : deptvals) {
            stmt->executeUpdate(s + val);
        }
        std::cout << "DEPT records inserted." << std::endl;

        // Create and populate COURSE table
        s = "create table COURSE(CId int, Title varchar(20), DeptId int)";
        stmt->executeUpdate(s);
        std::cout << "Table COURSE created." << std::endl;

        s = "insert into COURSE(CId, Title, DeptId) values ";
        std::vector<std::string> coursevals = {
            "(12, 'db systems', 10)",
            "(22, 'compilers', 10)",
            "(32, 'calculus', 20)",
            "(42, 'algebra', 20)",
            "(52, 'acting', 30)",
            "(62, 'elocution', 30)"
        };
        for (const auto& val : coursevals) {
            stmt->executeUpdate(s + val);
        }
        std::cout << "COURSE records inserted." << std::endl;

        // Create and populate SECTION table
        s = "create table SECTION(SectId int, CourseId int, Prof varchar(8), YearOffered int)";
        stmt->executeUpdate(s);
        std::cout << "Table SECTION created." << std::endl;

        s = "insert into SECTION(SectId, CourseId, Prof, YearOffered) values ";
        std::vector<std::string> sectvals = {
            "(13, 12, 'turing', 2018)",
            "(23, 12, 'turing', 2019)",
            "(33, 32, 'newton', 2019)",
            "(43, 32, 'einstein', 2017)",
            "(53, 62, 'brando', 2018)"
        };
        for (const auto& val : sectvals) {
            stmt->executeUpdate(s + val);
        }
        std::cout << "SECTION records inserted." << std::endl;

        // Create and populate ENROLL table
        s = "create table ENROLL(EId int, StudentId int, SectionId int, Grade varchar(2))";
        stmt->executeUpdate(s);
        std::cout << "Table ENROLL created." << std::endl;

        s = "insert into ENROLL(EId, StudentId, SectionId, Grade) values ";
        std::vector<std::string> enrollvals = {
            "(14, 1, 13, 'A')",
            "(24, 1, 43, 'C' )",
            "(34, 2, 43, 'B+')",
            "(44, 4, 33, 'B' )",
            "(54, 4, 53, 'A' )",
            "(64, 6, 53, 'A' )"
        };
        for (const auto& val : enrollvals) {
            stmt->executeUpdate(s + val);
        }
        std::cout << "ENROLL records inserted." << std::endl;
        conn->commit();
    }
    catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}