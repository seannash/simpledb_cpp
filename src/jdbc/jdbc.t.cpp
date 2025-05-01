
#include <catch2/catch_test_macros.hpp>

#include "jdbc.hpp"

namespace simpledb::jdbc {

TEST_CASE("MockDriver Ints", "[jdbc]") {
    MockDriver driver;
    Properties props;
    auto connection = driver.getConnection("jdbc:simpledb:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT 1");
    REQUIRE(resultSet->getInt(1) == 1);
}

TEST_CASE("MockDriver Strings", "[jdbc]") {
    MockDriver driver;
    Properties props;
    auto connection = driver.getConnection("jdbc:simpledb:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT name from cat");
    REQUIRE(resultSet->getMetaData().getColumnCount() == 1);
    REQUIRE(resultSet->getMetaData().getColumnName(1) == "name");
    REQUIRE(resultSet->getMetaData().getColumnType(1) == ColumnType::STRING);
    REQUIRE(resultSet->getMetaData().getColumnTypeName(1) == "varchar");
    REQUIRE(resultSet->getMetaData().getColumnDisplaySize(1) == 10);
    REQUIRE(resultSet->getString(1) == "gato");
    REQUIRE(resultSet->next());
    REQUIRE(resultSet->getString(1) == "perro");
    REQUIRE(!resultSet->next());
}

TEST_CASE("DriverManager Ints", "[jdbc]") {
    auto mockDriver = std::make_unique<MockDriver>();
    auto driverManager = std::make_unique<DriverManager>();
    driverManager->registerDriver(std::move(mockDriver));
    Properties props;
    REQUIRE(driverManager->supports("jdbc:simpledb:test"));
    auto connection = driverManager->getConnection("jdbc:simpledb:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT 1");
    REQUIRE(resultSet->getInt(1) == 1);
}

} // namespace simpledb::jdbc
