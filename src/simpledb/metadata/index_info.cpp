#include "index_info.hpp"
#include "simpledb/record/schema.hpp"


namespace simpledb::metadata {

Index::Index(std::shared_ptr<simpledb::tx::Transaction> tx, std::string_view index_name, std::shared_ptr<simpledb::record::Layout> index_layout)
{}

int Index::search_cost(int x, int y) {
    return 0;
}


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
    return Index::search_cost(num_blocks, rpb);
}

int IndexInfo::records_output() const {
    return d_stat_info.records_output() / d_stat_info.distinct_values(d_field_name);
}

int IndexInfo::distinct_values(std::string_view fname) const {
    return d_field_name == fname ? 1 : d_stat_info.distinct_values(d_field_name);
}

std::unique_ptr<Index> IndexInfo::open() {
    return std::make_unique<Index>(d_tx, d_index_name, d_index_layout);
}

std::shared_ptr<simpledb::record::Layout> IndexInfo::create_index_layout() {
    auto schema = std::make_shared<simpledb::record::Schema>();
    schema->add_int_field("block");
    schema->add_int_field("id");
    if (d_table_schema->get_type(d_field_name) == simpledb::record::FieldType::INT) {
        schema->add_int_field("dataval");
    } else {
        int field_len = d_table_schema->get_length(d_field_name);
        schema->add_string_field("dataval", field_len);
    }
    return std::make_shared<simpledb::record::Layout>(*schema);
}

} // namespace simpledb::metadata