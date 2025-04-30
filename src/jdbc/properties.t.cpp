#include "properties.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace simpledb::jdbc;

TEST_CASE("Properties class", "[jdbc]") {
    Properties props;

    SECTION("Setting and getting properties") {
        props.setProperty("user", "testuser");
        props.setProperty("password", "testpass");
        
        REQUIRE(props.getProperty("user") == "testuser");
        REQUIRE(props.getProperty("password") == "testpass");
    }

    SECTION("Getting non-existent property") {
        REQUIRE(props.getProperty("nonexistent").empty());
    }

    SECTION("Overwriting property") {
        props.setProperty("key", "value1");
        props.setProperty("key", "value2");
        
        REQUIRE(props.getProperty("key") == "value2");
    }
}

