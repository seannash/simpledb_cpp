#include "simpledb/multibuffer/MultibufferProductPlan.hpp"
#include "simpledb/materialize/MaterializePlan.hpp"
#include "simpledb/multibuffer/MultibufferProductScan.hpp"
#include "simpledb/query/update_scan.hpp"

namespace simpledb::multibuffer {

MultibufferProductPlan::MultibufferProductPlan(
    std::shared_ptr<simpledb::tx::Transaction> tx,
    std::shared_ptr<simpledb::plan::Plan> lhs,
    std::shared_ptr<simpledb::plan::Plan> rhs)
    : d_tx(std::move(tx))
    , d_lhs(std::make_shared<simpledb::materialize::MaterializePlan>(d_tx, std::move(lhs)))
    , d_rhs(std::move(rhs))
{
    d_schema.add_all(d_lhs->schema());
    d_schema.add_all(d_rhs->schema());
}

std::shared_ptr<simpledb::query::Scan> MultibufferProductPlan::open() {
    auto leftscan = d_lhs->open();
    auto t = copy_records_from(d_rhs);
    // FIXME: Should layout be a shared_ptr?
    return std::make_shared<MultibufferProductScan>(d_tx, leftscan, std::string(t.tableName()), *t.getLayout());
}

int MultibufferProductPlan::blocks_accessed() const{
    // this guesses at the # of chunks
    int avail = d_tx->available_buffers();
    int size = simpledb::materialize::MaterializePlan(d_tx, d_rhs).blocks_accessed();
    int numchunks = size / avail;
    return d_rhs->blocks_accessed() + (d_lhs->blocks_accessed() * numchunks);
}

int MultibufferProductPlan::records_output() const {
    return d_lhs->records_output() * d_rhs->records_output();
}

int MultibufferProductPlan::distinct_values(std::string_view field) const {
    if (d_lhs->schema().has_field(field)) {
        return d_lhs->distinct_values(field);
    } else {
        return d_rhs->distinct_values(field);
    }
}

const simpledb::record::Schema& MultibufferProductPlan::schema() const {
    return d_schema;
}

simpledb::materialize::TempTable MultibufferProductPlan::copy_records_from(std::shared_ptr<simpledb::plan::Plan> p) {
    auto src = p->open();
    auto sch = p->schema();
    // FIXME: this is an extra conversion to get around the fact that the schema() is not a shared_ptr
    simpledb::materialize::TempTable tt(d_tx, std::make_shared<simpledb::record::Schema>(sch));
    auto dest = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(tt.open());
    while (src->next()) {
        dest->insert();
        for (const auto& fldname : sch.fields()) {
            dest->set_val(fldname, src->get_val(fldname));
        }
    }
    
    src->close();
    dest->close();
    return tt;
}

} // namespace simpledb::multibuffer 