#include "simpledb/record/schema.hpp"
#include <span>

namespace simpledb::record {

Schema::Schema(): d_info() {
}

void Schema::add_field(std::string_view name, ::jdbc::ColumnType type, int length) {
    d_info.emplace(std::piecewise_construct,
                   std::forward_as_tuple(std::string(name)),
                   std::forward_as_tuple(type, length));
    d_fields.push_back(std::string(name));
}

void Schema::add_int_field(std::string_view name) {
    add_field(name, ::jdbc::ColumnType::INT, sizeof(int));
}

void Schema::add_string_field(std::string_view name, int length) {
    add_field(name, ::jdbc::ColumnType::STRING, length);
}

void Schema::add(std::string_view name, Schema schema) {
    auto type = schema.get_type(name);
    auto length = schema.get_length(name);
    add_field(name, type, length);
}

void Schema::add_all(const Schema& schema) {
    for (const auto& field : schema.fields()) {
        add_field(field, schema.get_type(field), schema.get_length(field));
    }
}

std::span<const std::string> Schema::fields() const {
    return d_fields;
}

bool Schema::has_field(std::string_view name) const {
    return d_info.find(std::string(name)) != d_info.end();
}

::jdbc::ColumnType Schema::get_type(std::string_view name) const {
    return d_info.find(std::string(name))->second.type();
}

int Schema::get_length(std::string_view name) const {
    return d_info.find(std::string(name))->second.length();
}

} // namespace simpledb::record

