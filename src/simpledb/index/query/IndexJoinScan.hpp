#pragma once

#include <memory>
#include <string>

#include "simpledb/query/scan.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/index/Index.hpp"
#include "simpledb/query/constant.hpp"

namespace simpledb::index::query {

class IndexJoinScan : public simpledb::query::Scan {
public:

   IndexJoinScan(std::shared_ptr<simpledb::query::Scan> lhs,
                 std::shared_ptr<simpledb::index::Index> idx,
                 std::string joinfield,
                 std::shared_ptr<simpledb::record::TableScan> rhs);
   

   void before_first() override;
   bool next() override;
   int get_int(std::string_view fldname) override;
   simpledb::query::Constant get_val(std::string_view fldname) override;
   std::string get_string(std::string_view fldname) override;
   bool has_field(std::string_view fldname) override;
   void close() override;

private:
   std::shared_ptr<simpledb::query::Scan> d_lhs;
   std::shared_ptr<simpledb::index::Index> d_idx;
   std::string d_joinfield;
   std::shared_ptr<simpledb::record::TableScan> d_rhs;

   void resetIndex();
};

} // namespace simpledb::index::query