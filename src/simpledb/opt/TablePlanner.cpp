#include "simpledb/opt/TablePlanner.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/plan/select_plan.hpp"
#include "simpledb/index/planner/IndexSelectPlan.hpp"
#include "simpledb/index/planner/IndexJoinPlan.hpp"
#include "simpledb/plan/plan.hpp"
#include <iostream>

namespace simpledb::opt {

TablePlanner::TablePlanner(
    std::string tblname,
    simpledb::query::Predicate mypred,
    std::shared_ptr<simpledb::tx::Transaction> tx,
    std::shared_ptr<simpledb::metadata::MetadataManager> mdm)
    : d_mypred(std::move(mypred))
    , d_tx(std::move(tx))
{
    d_myplan = std::make_shared<simpledb::plan::TablePlan>(d_tx, std::move(tblname), std::move(mdm));
    d_myschema = d_myplan->schema();
    d_indexes = mdm->get_index_info(tblname, d_tx);
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_select_plan() {
    auto p = make_index_select();
    if (!p) {
        p = d_myplan;
    }
    return add_select_pred(p);
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_join_plan(
    std::shared_ptr<simpledb::plan::Plan> current) {
    auto currsch = current->schema();
    auto joinpred = d_mypred.join_sub_pred(d_myschema, currsch);
    if (!joinpred) {
        return nullptr;
    }

    auto p = make_index_join(current, currsch);
    if (!p) {
        p = make_product_join(current, currsch);
    }
    return p;
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_product_plan(
    std::shared_ptr<simpledb::plan::Plan> current) {
    auto p = add_select_pred(d_myplan);
    return std::make_shared<simpledb::multibuffer::MultibufferProductPlan>(d_tx, current, p);
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_index_select() {
    for (const auto& [fldname, ii] : d_indexes) {
        auto val = d_mypred.equates_with_constant(fldname);
        if (!val.is_null()) {
            // FIXME: Correct extra shared_ptr creation
            return std::make_shared<simpledb::index::planner::IndexSelectPlan>(
                d_myplan, std::make_shared<simpledb::metadata::IndexInfo>(ii), val.as_string());
        }
    }
    return nullptr;
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_index_join(
      std::shared_ptr<simpledb::plan::Plan> current,
      const simpledb::record::Schema& currsch) {
    for (const auto& [fldname, ii] : d_indexes) {
        auto outerfield = d_mypred.equates_with_field(fldname);
        if (!outerfield.is_null() && currsch.has_field(outerfield.as_string())) {
            // FIXME: Correct extra shared_ptr creation
            auto p = std::dynamic_pointer_cast<simpledb::plan::Plan>(std::make_shared<simpledb::index::planner::IndexJoinPlan>(current, d_myplan, std::make_shared<simpledb::metadata::IndexInfo>(ii), outerfield.as_string()));
            p = add_select_pred(p);
            return add_join_pred(p, currsch);
        }
    }
    return nullptr;
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::make_product_join(
    std::shared_ptr<simpledb::plan::Plan> current,
    const simpledb::record::Schema& currsch) {
    auto p = make_product_plan(current);
    return add_join_pred(p, currsch);
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::add_select_pred(
    std::shared_ptr<simpledb::plan::Plan> p) {
    auto selectpred = d_mypred.select_sub_pred(d_myschema);
    if (selectpred) {
        return std::make_shared<simpledb::plan::SelectPlan>(p, selectpred);
    }
    return p;
}

std::shared_ptr<simpledb::plan::Plan> TablePlanner::add_join_pred(
    std::shared_ptr<simpledb::plan::Plan> p,
    const simpledb::record::Schema& currsch) {
    auto joinpred = d_mypred.join_sub_pred(currsch, d_myschema);
    if (joinpred) {
        return std::make_shared<simpledb::plan::SelectPlan>(p, joinpred);
    }
    return p;
}

} // namespace simpledb::opt 