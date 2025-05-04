
#include "simpledb/jdbc/embedded/embedded_driver.hpp"

#include <iostream>

int main() {
    simpledb::jdbc::embedded::EmbeddedDriver driver;
    auto conn = driver.connect("test", {});
    auto stmt = conn->createStatement();
    try {
        stmt->executeUpdate("create table test (sname varchar(20))");
        stmt->executeUpdate("insert into test (sname) values ('John')");
        stmt->executeUpdate("insert into test (sname) values ('Jane')");
        stmt->executeUpdate("insert into test (sname) values ('Jim')");
        auto rs = stmt->executeQuery("select sname from test");
        while (rs->next()) {
            std::cout << rs->getString("sname") << std::endl;
        }
    } catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}