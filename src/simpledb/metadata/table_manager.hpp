#pragma once

#include <memory>
#include <string_view>
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"

namespace simpledb::metadata {

class TableManager {
public:
    static constexpr int MAX_NAME_LEN = 16;
    TableManager(bool is_new, std::shared_ptr<simpledb::tx::Transaction> tx);
    ~TableManager() = default;

    void create_table(std::string_view table_name,
                     std::shared_ptr<simpledb::record::Schema> schema,
                     std::shared_ptr<simpledb::tx::Transaction> tx);
    std::shared_ptr<simpledb::record::Layout> get_layout(std::string_view table_name,
                                                        std::shared_ptr<simpledb::tx::Transaction> tx) const;

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_tcat_layout;
    std::shared_ptr<simpledb::record::Layout> d_fcat_layout;

};

} // namespace simpledb::metadata