#include "simpledb/server/simpledb.hpp"
#include "simpledb/plan/basic_query_planner.hpp"
#include "simpledb/plan/basic_update_planner.hpp"

#include <filesystem>

namespace {

const int DEFAULT_BLOCK_SIZE = 4096;
const int DEFAULT_BUFFER_SIZE = 4000;


}

namespace simpledb::server {

SimpleDB::SimpleDB(std::string_view db_directory, int block_size, int buffer_size)
    : d_db_directory(db_directory)
    , d_block_size(block_size)
    , d_buffer_size(buffer_size)
{
    bool is_new = !std::filesystem::exists(db_directory);


    // Initialize components in the correct order
    d_file_manager = std::make_shared<simpledb::file::FileManager>(db_directory, block_size);
    d_log_manager = std::make_shared<simpledb::log::LogManager>(d_file_manager, "logfile");
    d_buffer_manager = std::make_shared<simpledb::buffer::BufferManager>(d_file_manager, d_log_manager, buffer_size);
    auto tx = new_tx();
    d_metadata_manager = std::make_shared<simpledb::metadata::MetadataManager>(is_new, tx);
    tx->commit();
    std::shared_ptr<simpledb::plan::QueryPlanner> qplanner = std::make_shared<simpledb::plan::BasicQueryPlanner>(d_metadata_manager);
    std::shared_ptr<simpledb::plan::UpdatePlanner> uplanner = std::make_shared<simpledb::plan::BasicUpdatePlanner>(d_metadata_manager);
    d_planner = std::make_shared<simpledb::plan::Planner>(qplanner, uplanner);
}

SimpleDB::SimpleDB(std::string_view db_directory)
    : SimpleDB(db_directory, DEFAULT_BLOCK_SIZE, DEFAULT_BUFFER_SIZE)
{
}

std::shared_ptr<simpledb::file::FileManager> SimpleDB::file_manager() {
    return d_file_manager;
}

std::shared_ptr<simpledb::log::LogManager> SimpleDB::log_manager() {
    return d_log_manager;
}

std::shared_ptr<simpledb::buffer::BufferManager> SimpleDB::buffer_manager() {
    return d_buffer_manager;
}

std::shared_ptr<simpledb::tx::Transaction> SimpleDB::new_tx() {
    return std::make_shared<simpledb::tx::Transaction>(d_file_manager, d_log_manager, d_buffer_manager);
}

std::shared_ptr<simpledb::metadata::MetadataManager> SimpleDB::metadata_manager() {
    return d_metadata_manager;
}

std::shared_ptr<simpledb::plan::Planner> SimpleDB::planner() {
    return d_planner;
}

} // namespace simpledb::server
