#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/record_page.hpp"
#include "simpledb/tx/transaction.hpp"

#include <memory>
#include <string>
#include <vector>

namespace simpledb::multibuffer {

class ChunkScan : public simpledb::query::Scan {
public:

    ChunkScan(std::shared_ptr<simpledb::tx::Transaction> tx,
              std::string filename,
              simpledb::record::Layout layout,
              int startbnum,
              int endbnum);

    void close() override;
    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    simpledb::query::Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;

private:
    void move_to_block(int blknum);

    std::vector<std::shared_ptr<simpledb::record::RecordPage>> d_buffs;
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::string d_filename;
    simpledb::record::Layout d_layout;
    int d_startbnum;
    int d_endbnum;
    int d_currentbnum;
    std::shared_ptr<simpledb::record::RecordPage> d_rp;
    int d_currentslot;
};

} // namespace simpledb::multibuffer