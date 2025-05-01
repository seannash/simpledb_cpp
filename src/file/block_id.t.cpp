#include "block_id.hpp"

#include <catch2/catch_test_macros.hpp>

namespace simpledb::file {

TEST_CASE("BlockId", "[block_id]") {
    BlockId blockId("test", 1);
    //REQUIRE(blockId.fileName() == "test");
    //REQUIRE(blockId.number() == 1);
}

} // namespace simpledb::file