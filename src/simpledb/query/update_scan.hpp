#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/rid.hpp"

namespace simpledb::query {

class UpdateScan : public Scan {
public:

    ~UpdateScan() = default;

    virtual void set_int(std::string_view field, int val) = 0;
    virtual void set_string(std::string_view field, std::string_view val) = 0;
    virtual void set_val(std::string_view field, const std::any& val) = 0;
    
    virtual void insert() = 0;
    virtual void delete_record() = 0;
    virtual simpledb::record::RID current_rid() = 0;
    virtual void move_to_rid(simpledb::record::RID rid) = 0;

};

} // namespace simpledb::query