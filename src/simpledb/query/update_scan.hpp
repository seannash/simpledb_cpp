#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/record/Schema.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/RID.hpp"

namespace simpledb::query {

class UpdateScan : public Scan {
public:

    ~UpdateScan() = default;

    virtual void setInt(std::string_view field, int val) = 0;
    virtual void setString(std::string_view field, std::string_view val) = 0;
    virtual void setVal(std::string_view field, Constant val) = 0;
    
    virtual void insert() = 0;
    virtual void deleteRecord() = 0;
    virtual simpledb::record::RID getRid() = 0;
    virtual void moveToRID(simpledb::record::RID rid) = 0;

};

} // namespace simpledb::query