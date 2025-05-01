#include "page.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Page", "[page]") {
    simpledb::file::Page page(100);
    REQUIRE(page.contents()->size() == 100);

    page.setInt(0, 100);
    REQUIRE(page.getInt(0) == 100);

    page.setInt(1, 200);
    REQUIRE(page.getInt(1) == 200);
    
}