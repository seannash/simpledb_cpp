#pragma once

#include "simpledb/record/schema.hpp"
#include <unordered_map>
#include <string_view>

namespace simpledb::record {

class Layout {
public:
    Layout(Schema schema);
    Layout(Schema schema,
           std::unordered_map<std::string, int> offsets,
           int slot_size);

    int slot_size();
    int offset(std::string_view field);
    Schema& schema();
    
private:
    Schema d_schema;
    std::unordered_map<std::string, int> d_offsets;
    int d_slot_size;

    int length_in_bytes(std::string_view field);
};

} // namespace simpledb::record

