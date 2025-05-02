#pragma once

#include "simpledb/tx/transaction.hpp"
#include "simpledb/file/block_id.hpp"
#include "simpledb/record/layout.hpp"

namespace simpledb::record {

class RecordPage {
public:
    RecordPage(std::shared_ptr<simpledb::tx::Transaction> tx, simpledb::file::BlockId& blk, Layout layout);
    ~RecordPage() = default;

    int get_int(int slot, std::string_view field);
    std::string get_string(int slot, std::string_view field);
    void set_int(int slot, std::string_view field, int value);
    void set_string(int slot, std::string_view field, std::string_view value);
    void format();
    void delete_slot(int slot);
    
    int next_after(int slot);
    int insert_after(int slot);
    
    bool is_slot_valid(int slot);

    simpledb::file::BlockId block() const;
    
private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    simpledb::file::BlockId d_blk;
    Layout d_layout;

    int offset(int slot);
    
    void set_flag(int slot, int flag);
    int search_after(int slot, int flag);
};
} // namespace simpledb::record