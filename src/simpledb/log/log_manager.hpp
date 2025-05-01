#pragma once

#include <string>
#include <memory>
#include <span>

#include "simpledb/file/file_manager.hpp"
#include "simpledb/log/log_iterator.hpp"
#include "simpledb/file/page.hpp"

namespace simpledb::log {

class LogManager {
public:
    LogManager(std::shared_ptr<file::FileManager> fileManager, const std::string& logFilePath);
    ~LogManager() = default;

    int append(std::span<char> data);
    void flush(int lsn);
    void flush();
    LogIterator iterator();

private:
    std::shared_ptr<file::FileManager> d_fileManager;
    std::string d_logFile;
    file::Page d_log_page;
    file::BlockId d_currentBlock;
    int d_latest_lsn;
    int d_last_saved_lsn;

    file::BlockId appendNewBlock();
    
};

} // namespace simpledb::log
