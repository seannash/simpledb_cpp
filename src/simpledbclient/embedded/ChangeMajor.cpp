#include "simpledb/jdbc/embedded/embedded_driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/statement.hpp"
#include "jdbc/sql_exception.hpp"
#include <iostream>
#include <memory>

int main() {
    auto d = std::make_unique<simpledb::jdbc::embedded::EmbeddedDriver>();
    std::string url = "jdbc:simpledb:studentdb";

    try {
        auto conn = d->connect(url, {});
        auto stmt = conn->createStatement();

        std::string cmd = "update STUDENT "
                         "set MajorId=30 "
                         "where SName = 'amy'";
        stmt->executeUpdate(cmd);
        std::cout << "Amy is now a drama major." << std::endl;
    }
    catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}