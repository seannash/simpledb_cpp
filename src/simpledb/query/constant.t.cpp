#include "simpledb/query/constant.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace simpledb::query;

TEST_CASE("Constant") {
    Constant c1(1);
    Constant c2("hello");
}

TEST_CASE("Constant comparison") {
    Constant c1(1);
    Constant c2(2);
    Constant c3(1);
    Constant c4(std::string("hello"));
    Constant c5(std::string("world"));

    CHECK(c1 < c2);
    CHECK(c1 != c2);
    CHECK(c1 == c3);
    CHECK(c4 < c5);
    CHECK(c1 <=> c2 == std::strong_ordering::less);
    CHECK(c1 <=> c3 == std::strong_ordering::equal);
}