#pragma once

#include <string_view>

namespace simpledb::metadata {


class StatInfo {
public:
    StatInfo(int num_blocks, int num_records);
    ~StatInfo() = default;

    int blocks_accessed() const;
    int records_output() const;
    int distinct_values(std::string_view field) const;

private:
    int d_num_blocks;
    int d_num_records;
};

} // namespace simpledb::metadata