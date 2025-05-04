#include "simpledb/jdbc/embedded/embedded_driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/statement.hpp"
#include "jdbc/result_set.hpp"
#include "jdbc/sql_exception.hpp"
#include <iostream>
#include <memory>
#include <string>

int main() {
    std::string url = "studentdb";
    std::string qry = "select SName, DName "
                     "from DEPT, STUDENT "
                     "where MajorId = DId";

    auto d = std::make_unique<simpledb::jdbc::embedded::EmbeddedDriver>();
    try {
        auto conn = d->connect(url, {});
        auto stmt = conn->createStatement();

        std::cout << "Name\tMajor" << std::endl;
        auto rs = stmt->executeQuery(qry);
        while (rs->next()) {
            std::string sname = rs->getString("SName");
            std::string dname = rs->getString("DName");
            std::cout << sname << "\t" << dname << std::endl;
        }
        rs->close();
    }
    catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
