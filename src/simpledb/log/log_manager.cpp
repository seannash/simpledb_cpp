#include "simpledb/log/log_manager.hpp"
#include <cstring>

namespace simpledb::log {

LogManager::LogManager(std::shared_ptr<file::FileManager> fileManager, const std::string& logFilePath)
    : d_fileManager(fileManager)
    , d_logFile(logFilePath)
    , d_log_page(fileManager->blockSize())
    , d_currentBlock(file::BlockId(logFilePath, 0))
    , d_latest_lsn(0)
    , d_last_saved_lsn(0)
{
    int log_size = fileManager->length(logFilePath);
    if (log_size == 0) {
        d_currentBlock = appendNewBlock();
    } else {
        d_currentBlock = file::BlockId(logFilePath, log_size - 1);
        d_fileManager->read(d_currentBlock, &d_log_page);
    }
}

int LogManager::append(std::span<char> log_record) {
    int boundary = d_log_page.getInt(0);
    int record_size = log_record.size();
    int bytes_needed = record_size + sizeof(int);
    if (boundary - bytes_needed < sizeof(int)) {
        flush();
        d_currentBlock = appendNewBlock();
        boundary = d_log_page.getInt(0);
    }
    int rec_pos = boundary - bytes_needed;
    d_log_page.setBytes(rec_pos, log_record);
    d_log_page.setInt(0, rec_pos);
    d_latest_lsn++;
    return d_latest_lsn;
}

file::BlockId LogManager::appendNewBlock() {
    file::BlockId blk = d_fileManager->append(d_logFile);
    d_log_page.setInt(0, d_fileManager->blockSize());
    d_fileManager->write(blk, &d_log_page);
    return blk;
}

void LogManager::flush(int lsn) {
    if (lsn >= d_last_saved_lsn) {
        flush();
    }
}

void LogManager::flush() {
    d_fileManager->write(d_currentBlock, &d_log_page);
    d_last_saved_lsn = d_latest_lsn;
}

LogIterator LogManager::iterator() {
    flush();
    return LogIterator(d_fileManager, d_currentBlock);
}

} // namespace simpledb::log
