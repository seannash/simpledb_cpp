#include "simpledb/server/simpledb.hpp"
#include "simpledb/plan/basic_query_planner.hpp"
#include "simpledb/plan/basic_update_planner.hpp"

namespace simpledb::server {

SimpleDB::SimpleDB(std::string_view db_directory, int block_size, int buffer_size)
    : d_db_directory(db_directory)
    , d_block_size(block_size)
    , d_buffer_size(buffer_size)
{
    // Initialize components in the correct order
    d_file_manager = std::make_shared<simpledb::file::FileManager>(db_directory, block_size);
    d_log_manager = std::make_shared<simpledb::log::LogManager>(d_file_manager, "logfile");
    d_buffer_manager = std::make_shared<simpledb::buffer::BufferManager>(d_file_manager, d_log_manager, buffer_size);
    d_metadata_manager = std::make_shared<simpledb::metadata::MetadataManager>(true, new_tx());
    std::shared_ptr<simpledb::plan::QueryPlanner> qplanner = std::make_shared<simpledb::plan::BasicQueryPlanner>(d_metadata_manager);
    std::shared_ptr<simpledb::plan::UpdatePlanner> uplanner = std::make_shared<simpledb::plan::BasicUpdatePlanner>(d_metadata_manager);
    d_planner = std::make_shared<simpledb::plan::Planner>(qplanner, uplanner);
}

SimpleDB::SimpleDB(std::string_view db_directory)
    : SimpleDB(db_directory, 4096, 8) // Default values: 4KB block size, 8 buffers
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
