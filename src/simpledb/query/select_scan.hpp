#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/query/update_scan.hpp"
#include "simpledb/record/rid.hpp"

#include <memory>

class Predicate {
public:
    // Predicate class definition will be implemented later
    bool is_satisfied(std::shared_ptr<simpledb::query::Scan> s) const;
};

namespace simpledb::query {

class SelectScan : public UpdateScan {
public:
    SelectScan(std::shared_ptr<Scan> scan, Predicate pred);

    // Scan interface methods
    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;
    void close() override;    

    // UpdateScan interface methods
    void set_int(std::string_view field, int val) override;
    void set_string(std::string_view field, std::string_view val) override;
    void set_val(std::string_view field, Constant val) override;
    void insert() override;
    void delete_record() override;
    simpledb::record::RID current_rid() override;
    void move_to_rid(simpledb::record::RID rid) override;

private:
    std::shared_ptr<Scan> d_scan;
    std::shared_ptr<Predicate> d_predicate;
};

} // namespace simpledb::query