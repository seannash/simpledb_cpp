
#include <catch2/catch_test_macros.hpp>

#include "jdbc.hpp"

#include "fake_data_source.hpp"

namespace simpledb::jdbc {

TEST_CASE("FakeDataSource Ints", "[jdbc]") {
    FakeDataSource dataSource;
    dataSource.setDatabase("test");
    auto connection = dataSource.getConnection();
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT 1;");
    REQUIRE(resultSet->getInt(1) == 1);
    resultSet->close();
    statement->close();
    connection->close();
}

} // namespace simpledb::jdbc
