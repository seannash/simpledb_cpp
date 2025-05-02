#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/query/update_scan.hpp"
#include "simpledb/record/rid.hpp"

#include <span>
#include <string>
#include <vector>

namespace simpledb::query {

class ProjectScan : public Scan {
public:
    ProjectScan(std::shared_ptr<Scan> scan, std::span<std::string> fields);

    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    std::any get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;
    void close() override;

private:
    std::shared_ptr<Scan> d_scan;
    std::vector<std::string> d_fields;  
};

} // namespace simpledb::query