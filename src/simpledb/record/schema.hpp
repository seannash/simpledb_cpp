#pragma once

#include <string>
#include <vector>
#include <string_view>
#include "simpledb/record/field_type.hpp"
#include <unordered_map>
#include <span>

namespace simpledb::record {

class Schema {
public:
    Schema();

    void add_field(std::string_view name, FieldType type, int length);
    void add_int_field(std::string_view name);
    void add_string_field(std::string_view name, int length);
    void add(std::string_view name, Schema schema);
    void add_all(const Schema& schema);

    std::span<const std::string> fields() const;
    bool has_field(std::string_view name) const;
    FieldType get_type(std::string_view name) const;
    int get_length(std::string_view name) const;

private:

    class FieldInfo {
    public:
        FieldInfo(FieldType type, int length) : d_type(type), d_length(length) {}
        FieldType type() const { return d_type; }
        int length() const { return d_length; }
    private:
        FieldType d_type;
        int d_length;
    };

    std::unordered_map<std::string, FieldInfo> d_info;
    std::vector<std::string> d_fields;

};

} // namespace simpledb::record

