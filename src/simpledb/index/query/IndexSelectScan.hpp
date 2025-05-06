#pragma once

#include <memory>

#include "simpledb/query/scan.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/index/Index.hpp"
#include "simpledb/query/constant.hpp"

namespace simpledb::index::query {

class IndexSelectScan : public simpledb::query::Scan {
public:

    IndexSelectScan(std::shared_ptr<simpledb::record::TableScan> ts,
                   std::shared_ptr<simpledb::index::Index> idx,
                   const simpledb::query::Constant& val);

    void before_first() override;
    bool next() override;
    simpledb::query::Constant get_val(std::string_view fldname) override;
    int get_int(std::string_view fldname) override;
    std::string get_string(std::string_view fldname) override;
    bool has_field(std::string_view fldname) override;
    void close() override;

private:
    std::shared_ptr<simpledb::record::TableScan> d_ts;
    std::shared_ptr<simpledb::index::Index> d_idx;
    simpledb::query::Constant d_val;
};

} // namespace simpledb::index::planner