#pragma once

#include "simpledb/file/file_manager.hpp"
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/page.hpp"
#include "simpledb/file/block_id.hpp"

namespace simpledb::buffer {

class Buffer {
public:
    Buffer(std::shared_ptr<simpledb::file::FileManager> file_manager, std::shared_ptr<simpledb::log::LogManager> log_manager);
    ~Buffer();
    simpledb::file::Page& contents();
    simpledb::file::BlockId block();
    bool is_pinned();
    void set_modified(int txnum, int lsn);
    int modifing_tx();

    void assign_to_block(const simpledb::file::BlockId& blk);
    void flush();
    void pin();
    void unpin();

private:
    std::shared_ptr<simpledb::file::FileManager> d_file_manager;
    std::shared_ptr<simpledb::log::LogManager> d_log_manager;
    simpledb::file::Page d_page;
    int d_pins;
    int d_modifying_tx;
    int d_modifying_lsn;
    simpledb::file::BlockId d_block_id;


};

} // namespace simpledb::buffer

