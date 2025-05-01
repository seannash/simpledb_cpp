
#include <catch2/catch_test_macros.hpp>

#include "jdbc.hpp"

namespace simpledb::jdbc {

TEST_CASE("MockDriver", "[jdbc]") {
    MockDriver driver;
    Properties props;
    auto connection = driver.connect("jdbc:mock:test", props);
    auto statement = connection->createStatement();
    auto resultSet = statement->executeQuery("SELECT 1");
    REQUIRE(resultSet->getInt(1) == 1);
}

} // namespace simpledb::jdbc
