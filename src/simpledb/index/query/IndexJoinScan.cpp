#include "simpledb/index/query/IndexJoinScan.hpp"

namespace simpledb::index::query {

IndexJoinScan::IndexJoinScan(std::shared_ptr<simpledb::query::Scan> lhs,
                           std::shared_ptr<simpledb::index::Index> idx,
                           std::string joinfield,
                           std::shared_ptr<simpledb::record::TableScan> rhs)
: d_lhs(std::move(lhs))
, d_idx(std::move(idx))
, d_joinfield(std::move(joinfield))
, d_rhs(std::move(rhs))
{
    before_first();
}

void IndexJoinScan::before_first()
{
    d_lhs->before_first();
    d_lhs->next();
    resetIndex();
}

bool IndexJoinScan::next()
{
    while (true) {
        if (d_idx->next()) {
            d_rhs->move_to_rid(d_idx->getDataRid());
            return true;
        }
        if (!d_lhs->next()) {
            return false;
        }
        resetIndex();
    }
}

int IndexJoinScan::get_int(std::string_view fldname)
{
    if (d_rhs->has_field(fldname)) {
        return d_rhs->get_int(fldname);
    } else {
        return d_lhs->get_int(fldname);
    }
}

simpledb::query::Constant IndexJoinScan::get_val(std::string_view fldname)
{
    if (d_rhs->has_field(fldname)) {
        return d_rhs->get_val(fldname);
    } else {
        return d_lhs->get_val(fldname);
    }
}

std::string IndexJoinScan::get_string(std::string_view fldname)
{
    if (d_rhs->has_field(fldname)) {
        return d_rhs->get_string(fldname);
    } else {
        return d_lhs->get_string(fldname);
    }
}

bool IndexJoinScan::has_field(std::string_view fldname)
{
    return d_rhs->has_field(fldname) || d_lhs->has_field(fldname);
}

void IndexJoinScan::close()
{
    d_lhs->close();
    d_idx->close();
    d_rhs->close();
}

void IndexJoinScan::resetIndex()
{
    auto searchkey = d_lhs->get_val(d_joinfield);
    d_idx->beforeFirst(searchkey);
}

} // namespace simpledb::index::query 