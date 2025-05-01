#include "file_manager.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
namespace simpledb::file {

TEST_CASE("FileManager", "[file_manager]") {
    FileManager fileManager("q", 4096);
    BlockId blockId = fileManager.append("test.txt");
    REQUIRE(blockId.fileName() == "test.txt");
    REQUIRE(blockId.number() == 0);
    REQUIRE(std::filesystem::exists("q/test.txt"));

    Page page(4096);
    page.setInt(50, 42);
    fileManager.write(blockId, &page);

    Page page2(4096);
    fileManager.read(blockId, &page2);
    REQUIRE(page2.getInt(50) == 42);
}

} // namespace simpledb::file