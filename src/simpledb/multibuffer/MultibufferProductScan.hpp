#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/query/product_scan.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/tx/transaction.hpp"

#include <memory>
#include <string>

namespace simpledb::multibuffer {

class MultibufferProductScan : public simpledb::query::Scan {
public:

    MultibufferProductScan(std::shared_ptr<simpledb::tx::Transaction> tx,
                          std::shared_ptr<simpledb::query::Scan> lhsscan,
                          std::string tblname,
                          simpledb::record::Layout layout);

    void close() override;
    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    simpledb::query::Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;

private:
    bool use_next_chunk();

    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::query::Scan> d_lhsscan;
    std::shared_ptr<simpledb::query::Scan> d_rhsscan;
    std::shared_ptr<simpledb::query::Scan> d_prodscan;
    std::string d_filename;
    simpledb::record::Layout d_layout;
    int d_chunksize;
    int d_nextblknum;
    int d_filesize;
};

} // namespace simpledb::multibuffer 