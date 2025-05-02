#pragma once

#include "simpledb/query/scan.hpp"

#include <memory>

namespace simpledb::query {

class ProductScan : public Scan {
public:
    ProductScan(std::shared_ptr<Scan> left_scan, std::shared_ptr<Scan> right_scan);

    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    std::any get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;
    void close() override;

private:
    std::shared_ptr<Scan> d_left_scan;
    std::shared_ptr<Scan> d_right_scan;
};

} // namespace simpledb::query