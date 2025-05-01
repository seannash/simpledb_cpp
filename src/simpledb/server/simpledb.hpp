#pragma once

#include "simpledb/tx/transaction.hpp"
#include "simpledb/buffer/buffer_manager.hpp"
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/file_manager.hpp"

namespace simpledb::server {

class SimpleDB {
public:
    SimpleDB(std::string_view db_directory, int block_size, int buffer_size);
    SimpleDB(std::string_view db_directory);
    ~SimpleDB() = default;

    // std::shared_ptr<simpledb::tx::Transaction> transaction();

    std::shared_ptr<simpledb::file::FileManager> file_manager();
    std::shared_ptr<simpledb::log::LogManager> log_manager();
    std::shared_ptr<simpledb::buffer::BufferManager> buffer_manager();

private:
    std::string d_db_directory;
    int d_block_size;
    int d_buffer_size;
    std::shared_ptr<simpledb::file::FileManager> d_file_manager;
    std::shared_ptr<simpledb::log::LogManager> d_log_manager;
    std::shared_ptr<simpledb::buffer::BufferManager> d_buffer_manager;
    
};

} // namespace simpledb::server
