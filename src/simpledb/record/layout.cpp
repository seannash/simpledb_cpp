#include "simpledb/record/layout.hpp"
#include "simpledb/record/schema.hpp"
#include "jdbc/column_types.hpp"
#include <stdexcept>

namespace simpledb::record {

Layout::Layout(Schema schema):
    d_schema(schema),
    d_offsets(),
    d_slot_size(sizeof(int)) {
    for (const auto& field : schema.fields()) {
        d_offsets[field] = d_slot_size;
        d_slot_size += length_in_bytes(field);
    }
}

Layout::Layout(Schema schema,
               std::unordered_map<std::string, int> offsets,
               int slot_size)
    : d_schema(schema)
    , d_offsets(std::move(offsets))
    , d_slot_size(slot_size) {
}

int Layout::slot_size() {
    return d_slot_size;
}

int Layout::offset(std::string_view field) {
    return d_offsets[std::string(field)];
}

Schema& Layout::schema() {
    return d_schema;
}

int Layout::length_in_bytes(std::string_view field) {
    auto type = d_schema.get_type(field);
    switch (type) {
        case ::jdbc::ColumnType::INT:
            return sizeof(int);
        case ::jdbc::ColumnType::STRING:
            return d_schema.get_length(field)+sizeof(int);
    }
    throw std::runtime_error("Invalid type");
}
} // namespace simpledb::record
