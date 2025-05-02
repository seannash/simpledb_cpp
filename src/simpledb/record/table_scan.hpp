#pragma once

#include <memory>
#include <string_view>

#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"

namespace simpledb::record {

class TableScan {
public:
    TableScan(std::shared_ptr<simpledb::tx::Transaction> tx,
              std::string_view table_name,
              Layout layout);

    void close();
    bool has_field(std::string_view field);
    
    void before_first();
    bool next();
    void move_to_rid(RID rid);
    void insert();

    int get_int(std::string_view field);
    std::string get_string(std::string_view field);
    void set_int(std::string_view field, int value);
    void set_string(std::string_view field, std::string_view value);
    RID current_rid();
    void delete_record();

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::string d_table_name;
    Layout d_layout;

};


} // namespace simpledb::record