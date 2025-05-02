#include "simpledb/metadata/stat_info.hpp"

namespace simpledb::metadata {

StatInfo::StatInfo(int num_blocks, int num_records)
: d_num_blocks(num_blocks), d_num_records(num_records) {}

int StatInfo::blocks_accessed() const {
    return d_num_blocks;
}

int StatInfo::records_output() const {
    return d_num_records;
}

int StatInfo::distinct_values(std::string_view field) const {
    return 1 + (d_num_records / 3); // TODO: Not accurate
}

} // namespace simpledb::metadata