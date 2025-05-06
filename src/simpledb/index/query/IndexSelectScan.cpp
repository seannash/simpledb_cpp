#include "simpledb/index/query/IndexSelectScan.hpp"

namespace simpledb::index::query {

IndexSelectScan::IndexSelectScan(std::shared_ptr<simpledb::record::TableScan> ts,
                                std::shared_ptr<simpledb::index::Index> idx,
                                const simpledb::query::Constant& val)
: d_ts(std::move(ts))
, d_idx(std::move(idx))
, d_val(val)
{
    before_first();
}

void IndexSelectScan::before_first()
{
    d_idx->beforeFirst(d_val);
}

bool IndexSelectScan::next()
{
    bool ok = d_idx->next();
    if (ok) {
        auto rid = d_idx->getDataRid();
        d_ts->move_to_rid(rid);
    }
    return ok;
}

simpledb::query::Constant IndexSelectScan::get_val(std::string_view fldname)
{
    return d_ts->get_val(fldname);
}

int IndexSelectScan::get_int(std::string_view fldname)
{
    return d_ts->get_int(fldname);
}

std::string IndexSelectScan::get_string(std::string_view fldname)
{
    return d_ts->get_string(fldname);
}

bool IndexSelectScan::has_field(std::string_view fldname)
{
    return d_ts->has_field(fldname);
}

void IndexSelectScan::close()
{
    d_idx->close();
    d_ts->close();
}

} // namespace simpledb::index::planner 