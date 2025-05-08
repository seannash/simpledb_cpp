#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/query/predicate.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/multibuffer/MultibufferProductPlan.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace simpledb::opt {

class TablePlanner {
public:

   TablePlanner(std::string tblname,
                simpledb::query::Predicate mypred,
                std::shared_ptr<simpledb::tx::Transaction> tx,
                std::shared_ptr<simpledb::metadata::MetadataManager> mdm);
   
   std::shared_ptr<simpledb::plan::Plan> make_select_plan();
   
   std::shared_ptr<simpledb::plan::Plan> make_join_plan(
       std::shared_ptr<simpledb::plan::Plan> current);
   
   std::shared_ptr<simpledb::plan::Plan> make_product_plan(
       std::shared_ptr<simpledb::plan::Plan> current);
   
private:
   std::shared_ptr<simpledb::plan::Plan> make_index_select();
   std::shared_ptr<simpledb::plan::Plan> make_index_join(
       std::shared_ptr<simpledb::plan::Plan> current,
       const simpledb::record::Schema& currsch);
   std::shared_ptr<simpledb::plan::Plan> make_product_join(
       std::shared_ptr<simpledb::plan::Plan> current,
       const simpledb::record::Schema& currsch);
   std::shared_ptr<simpledb::plan::Plan> add_select_pred(
       std::shared_ptr<simpledb::plan::Plan> p);
   std::shared_ptr<simpledb::plan::Plan> add_join_pred(
       std::shared_ptr<simpledb::plan::Plan> p,
       const simpledb::record::Schema& currsch);

   std::shared_ptr<simpledb::plan::Plan> d_myplan;
   simpledb::query::Predicate d_mypred;
   simpledb::record::Schema d_myschema;
   std::map<std::string, simpledb::metadata::IndexInfo> d_indexes;
   std::shared_ptr<simpledb::tx::Transaction> d_tx;
};

} // namespace simpledb::opt