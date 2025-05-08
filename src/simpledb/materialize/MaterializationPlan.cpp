#include "simpledb/materialize/MaterializationPlan.hpp"
#include "simpledb/materialize/TempTable.hpp"

#include <cmath>

namespace simpledb::materialize {

MaterializationPlan::MaterializationPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                                       std::shared_ptr<simpledb::plan::Plan> srcplan)
: d_tx(std::move(tx))
, d_srcplan(std::move(srcplan))
{
}

std::shared_ptr<simpledb::query::Scan> MaterializationPlan::open() {
    auto sch = d_srcplan->schema();
    // FIXME: this is a hack to get around the fact that the schema() is not a shared_ptr
    auto sch_ptr = std::make_shared<simpledb::record::Schema>(sch);
    std::shared_ptr<simpledb::materialize::TempTable> temp_table = std::make_shared<TempTable>(d_tx, sch_ptr);
    auto src = d_srcplan->open();
    auto dest = temp_table->open();
    while (src->next()) {
        dest->insert();
        for (const auto& fldname : sch.fields()) {
            dest->set_val(fldname, src->get_val(fldname));
        }
    }
    src->close();
    dest->before_first();
    return dest;
}

int MaterializationPlan::blocks_accessed() const {
    simpledb::record::Layout layout {d_srcplan->schema()};
    double rbp = (double) (d_tx->block_size() / layout.slot_size());
    return (int) std::ceil(d_srcplan->blocks_accessed() * rbp);
}

int MaterializationPlan::records_output() const {
    return d_srcplan->records_output();
}

int MaterializationPlan::distinct_values(std::string_view fldname) const {
    return d_srcplan->distinct_values(fldname);
}

const simpledb::record::Schema& MaterializationPlan::schema() const {
    return d_srcplan->schema();
}

} // namespace simpledb::materialize 