#include "file_manager.hpp"

#include <filesystem>
#include <iostream>
namespace simpledb::file {

FileManager::FileManager(std::string_view dirname, int blockSize)
: d_dirname(dirname), d_blockSize(blockSize), d_isNew(false) {
    if (!std::filesystem::exists(d_dirname)) {
        std::filesystem::create_directories(d_dirname);
        d_isNew = true;
    }
    for (const auto& entry : std::filesystem::recursive_directory_iterator(d_dirname)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            const auto& filename = path.filename().string();
            
            if (filename.starts_with("temp_")) {
                std::filesystem::remove(path);
            }
        }
    }
}

FileManager::~FileManager() {
    for (auto& file : d_openFiles) {
        file.second->close();
    }
}

void FileManager::read(const BlockId& blockId, Page* page) {
    auto file = getFile(blockId.fileName());
    file->seekg(blockId.number() * d_blockSize);
    file->read(page->contents()->data(), d_blockSize);
}

void FileManager::write(const BlockId& blockId, Page* page) {
    auto file = getFile(blockId.fileName());
    file->seekp(blockId.number() * d_blockSize);
    file->write(page->contents()->data(), d_blockSize);
}

BlockId FileManager::append(std::string filename) {
    int new_block_number = length(filename);
    BlockId blkId{filename, new_block_number};
    std::vector<char> buffer(d_blockSize, 0);
    auto file = getFile(filename);
    if (!file->good()) {
        throw std::runtime_error("FileManager::append: file is not good");
    }
    file->seekp(0, std::ios::end);
    file->write(buffer.data(), d_blockSize);
    file->flush();
    return blkId;
}

unsigned int FileManager::length(std::string fileName) {
    auto file = getFile(fileName);
    file->seekg(0, std::ios::end);
    return file->tellg() / d_blockSize;
}   

bool FileManager::isNew() const {
    return d_isNew;
}   

int FileManager::blockSize() const {
    return d_blockSize;
}

std::shared_ptr<std::fstream> FileManager::getFile(std::string fileName) {
    auto iter = d_openFiles.find(fileName);
    if (iter != d_openFiles.end()) {
        return iter->second;
    }
    auto full_file_name = d_dirname + "/" + fileName;
    std::cout << "AAA" << full_file_name << std::endl;
    auto file = std::make_shared<std::fstream>(full_file_name, std::fstream::in | std::fstream::out | std::fstream::trunc );
    if (!file->good()) {
        throw std::runtime_error("FileManager::getFile: file is not good");
    }
    d_openFiles.emplace(fileName, file);
    return file;
}

}
