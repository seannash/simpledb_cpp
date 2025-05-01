#pragma once

#include <span>

#include "simpledb/file/page.hpp"
#include "simpledb/file/block_id.hpp"
#include "simpledb/file/file_manager.hpp"

namespace simpledb::log {

class LogIterator {
public:
    LogIterator(std::shared_ptr<file::FileManager> fileManager, file::BlockId blk);
    ~LogIterator();

    bool hasNext();
    std::vector<char> next();

    void moveToBlock(file::BlockId blk);

private:
    std::shared_ptr<file::FileManager> d_fileManager;
    file::BlockId d_blk;
    file::Page d_page;
    int d_currentPos;
    int d_boundary;
};

} // namespace simpledb::log