#include "simpledb/jdbc/embedded/embedded_driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/statement.hpp"
#include "jdbc/result_set.hpp"
#include "jdbc/sql_exception.hpp"
#include <iostream>
#include <memory>
#include <string>

int main() {
    std::cout << "Enter a department name: ";
    std::string major;
    std::getline(std::cin, major);

    std::cout << "Here are the " << major << " majors" << std::endl;
    std::cout << "Name\tGradYear" << std::endl;

    std::string url = "studentdb";
    std::string qry = "select sname, gradyear "
                      "from student, dept "
                      "where did = majorid "
                      "and dname = '" + major + "'";

    auto d = std::make_unique<simpledb::jdbc::embedded::EmbeddedDriver>();
    try {
        auto conn = d->connect(url, {});
        auto stmt = conn->createStatement();
        auto rs = stmt->executeQuery(qry);

        while (rs->next()) {
            std::string sname = rs->getString("sname");
            int gradyear = rs->getInt("gradyear");
            std::cout << sname << "\t" << gradyear << std::endl;
        }
    }
    catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}