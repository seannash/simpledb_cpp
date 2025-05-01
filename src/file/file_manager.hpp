#pragma once

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <mutex>
#include <fstream>

#include "block_id.hpp"
#include "page.hpp"

namespace simpledb::file {

class FileManager {
public:
    FileManager(std::string_view dirname, int blockSize);
    ~FileManager();

    void read(const BlockId& blockId, Page* page);
    void write(const BlockId& blockId, Page* page);
    BlockId append(std::string filename);

    unsigned int length(std::string fileName);

    bool isNew() const;

    int blockSize() const;


private:
    std::mutex d_mutex;
    std::string d_dirname;
    int d_blockSize;
    bool d_isNew;
    std::map<std::string, std::shared_ptr<std::fstream>> d_openFiles;

    std::shared_ptr<std::fstream> getFile(std::string fileName);
};

} // namespace simpledb::file

