#include "simpledb/log/log_iterator.hpp"

#include <iostream>
#include <span>

namespace simpledb::log {

LogIterator::LogIterator(std::shared_ptr<file::FileManager> fileManager, file::BlockId blk)
    : d_fileManager(fileManager)
    , d_blk(blk)
    , d_page(fileManager->blockSize())
    , d_currentPos(0)
    , d_boundary(0)
{
    moveToBlock(blk);
}

LogIterator::~LogIterator() = default;

bool LogIterator::hasNext() {
    return d_currentPos < d_fileManager->blockSize() || d_blk.number() > 0;
}

std::vector<char> LogIterator::next() {
    if (d_currentPos == d_fileManager->blockSize()) {
        moveToBlock(file::BlockId(d_blk.fileName(), d_blk.number() - 1));
    }
    std::vector<char> record = d_page.getBytes(d_currentPos);
    d_currentPos += record.size() + sizeof(int);
    return record;
}

void LogIterator::moveToBlock(file::BlockId blk) {
    d_blk = blk;
    d_fileManager->read(d_blk, &d_page);
    d_boundary = d_page.getInt(0);
    d_currentPos = d_boundary;
}

} // namespace simpledb::log
