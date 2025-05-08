#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <functional>

namespace simpledb::file {

class BlockId {
public:
    BlockId(std::string_view filename, int blockNumber);
    
    BlockId(const BlockId& other) = default;
    BlockId& operator=(const BlockId& other) = default;

    std::string fileName() const;
    int number() const;
    
    bool operator==(const BlockId& other) const;
    bool operator!=(const BlockId& other) const;
    
    std::string toString() const;
    
private:
    std::string d_filename;
    int d_blockNumber;
};

std::ostream& operator<<(std::ostream& os, const BlockId& blockId);

} // namespace simpledb::file

// Hash specialization for BlockId
namespace std {
    template<>
    struct hash<simpledb::file::BlockId> {
        size_t operator()(const simpledb::file::BlockId& blk) const {
            return hash<string>()(blk.fileName()) ^ (hash<int>()(blk.number()) << 1);
        }
    };
}