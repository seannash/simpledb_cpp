#include "simpledb/query/project_scan.hpp"
#include <algorithm>
#include <stdexcept>

namespace simpledb::query {

ProjectScan::ProjectScan(std::shared_ptr<Scan> scan, std::span<std::string> fields)
: d_scan(scan)
, d_fields(fields.begin(), fields.end())
{}

void ProjectScan::before_first() {
    d_scan->before_first();
}

bool ProjectScan::next() {
    return d_scan->next();
}

int ProjectScan::get_int(std::string_view field) {
    if (!has_field(field)) {
        throw std::runtime_error("Field not in projection: " + std::string(field));
    }
    return d_scan->get_int(field);
}

std::string ProjectScan::get_string(std::string_view field) {
    if (!has_field(field)) {
        throw std::runtime_error("Field not in projection: " + std::string(field));
    }
    return d_scan->get_string(field);
}

simpledb::query::Constant ProjectScan::get_val(std::string_view field) {
    if (!has_field(field)) {
        throw std::runtime_error("Field not in projection: " + std::string(field));
    }
    return d_scan->get_val(field);
}

bool ProjectScan::has_field(std::string_view field) {
    std::string lower_field = std::string(field);
    std::transform(lower_field.begin(), lower_field.end(), lower_field.begin(), ::tolower);
    return std::find(d_fields.begin(), d_fields.end(), lower_field) != d_fields.end();
}

void ProjectScan::close() {
    d_scan->close();
}

} // namespace simpledb::query
