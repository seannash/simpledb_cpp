#pragma once

#include <memory>
#include <string_view>
#include <any>
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/record/record_page.hpp"

namespace simpledb::record {

class TableScan {
public:
    TableScan(std::shared_ptr<simpledb::tx::Transaction> tx,
              std::string_view table_name,
              std::shared_ptr<Layout> layout);

    void close();
    bool has_field(std::string_view field);
    
    void before_first();
    bool next();
    void move_to_rid(RID rid);
    void insert();

    int get_int(std::string_view field);
    std::string get_string(std::string_view field);
    std::any get_val(std::string_view field);
    void set_int(std::string_view field, int value);
    void set_string(std::string_view field, std::string_view value);
    RID current_rid();
    void delete_record();

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_layout;
    std::shared_ptr<simpledb::record::RecordPage> d_rp;
    std::string d_filename;;
    int d_current_slot;

    void move_to_new_block();
    void move_to_block(int block_number);
    bool at_last_block();
    void set_val(std::string_view field, const std::any& value);
};


} // namespace simpledb::record