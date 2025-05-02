#pragma once

#include <memory>
#include <string_view>
#include <any>
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/record/record_page.hpp"
#include "simpledb/query/update_scan.hpp"

namespace simpledb::record {

class TableScan : public simpledb::query::UpdateScan {
public:
    TableScan(std::shared_ptr<simpledb::tx::Transaction> tx,
              std::string_view table_name,
              std::shared_ptr<Layout> layout);

    void close();
    bool has_field(std::string_view field);
    
    void before_first() override;
    bool next() override;
    void move_to_rid(RID rid) override;
    void insert() override;

    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    std::any get_val(std::string_view field) override;
    void set_int(std::string_view field, int value) override;
    void set_string(std::string_view field, std::string_view value) override;
    void set_val(std::string_view field, const std::any& value) override;
    RID current_rid() override;
    
    void delete_record() override;

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_layout;
    std::shared_ptr<simpledb::record::RecordPage> d_rp;
    std::string d_filename;;
    int d_current_slot;

    void move_to_new_block();
    void move_to_block(int block_number);
    bool at_last_block();
    
};


} // namespace simpledb::record