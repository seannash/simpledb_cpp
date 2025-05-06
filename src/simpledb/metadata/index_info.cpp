#include "index_info.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/index/Index.hpp"
#include "simpledb/index/hash/HashIndex.hpp"
#include "simpledb/index/btree/BTreeIndex.hpp"
#include "jdbc/column_types.hpp"

namespace simpledb::metadata {

IndexInfo::IndexInfo(
    std::string_view iname,
    std::string_view fname,
    std::shared_ptr<simpledb::record::Schema> table_schema,
    std::shared_ptr<simpledb::tx::Transaction> tx,
    StatInfo stat_info)
: d_index_name(iname)
, d_field_name(fname)
, d_tx(tx)
, d_table_schema(table_schema)
, d_stat_info(stat_info) {
    d_index_layout = create_index_layout();
}

int IndexInfo::blocks_accessed() const {
    int rpb = d_tx->block_size() / d_index_layout->slot_size();
    int num_blocks = d_stat_info.records_output() / rpb;
    // TODO: This depends on the index type and is codes to a static function in the index subclass
    // https://github.com/seannash/sciore-simpledb-3.4/blob/473a93d5bda1d8eac85491d52656c5633cbc60a8/SimpleDB_3.4/simpledb/metadata/IndexInfo.java#L67
    return num_blocks / 2; // FIXME: Lets divide by 42 for now
}

int IndexInfo::records_output() const {
    return d_stat_info.records_output() / d_stat_info.distinct_values(d_field_name);
}

int IndexInfo::distinct_values(std::string_view fname) const {
    return d_field_name == fname ? 1 : d_stat_info.distinct_values(d_field_name);
}

std::shared_ptr<simpledb::index::Index> IndexInfo::open() {
    // FIXME: Make switchable with BTreeIndex
    return std::make_shared<simpledb::index::hash::HashIndex>(d_tx, d_index_name, d_index_layout);
}

std::shared_ptr<simpledb::record::Layout> IndexInfo::create_index_layout() {
    auto schema = std::make_shared<simpledb::record::Schema>();
    schema->add_int_field("block");
    schema->add_int_field("id");
    if (d_table_schema->get_type(d_field_name) == ::jdbc::ColumnType::INT) {
        schema->add_int_field("dataval");
    } else {
        int field_len = d_table_schema->get_length(d_field_name);
        schema->add_string_field("dataval", field_len);
    }
    return std::make_shared<simpledb::record::Layout>(*schema);
}

} // namespace simpledb::metadata