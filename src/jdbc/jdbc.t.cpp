
#include <catch2/catch_test_macros.hpp>

#include "jdbc.hpp"

namespace simpledb::jdbc {

TEST_CASE("MockDriver Ints", "[jdbc]") {
    MockDriver driver;
    Properties props;
    auto connection = driver.connect("jdbc:mock:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT 1");
    REQUIRE(resultSet->getInt(1) == 1);
}

TEST_CASE("MockDriver Strings", "[jdbc]") {
    MockDriver driver;
    Properties props;
    auto connection = driver.connect("jdbc:mock:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT name from cat");
    REQUIRE(resultSet->getString(1) == "gato");
    REQUIRE(resultSet->next());
    REQUIRE(resultSet->getString(1) == "perro");
    REQUIRE(!resultSet->next());
}

} // namespace simpledb::jdbc
