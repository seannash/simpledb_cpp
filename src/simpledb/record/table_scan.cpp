#include "simpledb/record/table_scan.hpp"
#include <stdexcept>
#include <any>

namespace simpledb::record {

TableScan::TableScan(std::shared_ptr<simpledb::tx::Transaction> tx,
                     std::string_view table_name,
                     std::shared_ptr<Layout> layout)
: d_tx(tx)
, d_layout(layout)
, d_filename(std::string(table_name) + ".tbl")
, d_current_slot(-1) {
    if (tx->size(d_filename) == 0) {
        move_to_new_block();
    } else {
         move_to_block(0);
    }
}

void TableScan::close() {
    if (d_rp) {
        d_tx->unpin(d_rp->block());
    }
}

bool TableScan::has_field(std::string_view field) {
    return false;
}

void TableScan::before_first() {
    move_to_block(0);
}

bool TableScan::next() {
    d_current_slot = d_rp->next_after(d_current_slot);
    while (d_current_slot < 0) {
        if (at_last_block()) {
            return false;
        }
        move_to_block(d_rp->block().number() + 1);
        d_current_slot = d_rp->next_after(d_current_slot);
    }
    return true;
}

void TableScan::move_to_rid(RID rid) {
    close();
    auto blk = simpledb::file::BlockId(d_filename, rid.block_number());
    d_rp = std::make_shared<RecordPage>(d_tx, blk, *d_layout);
    d_current_slot = rid.slot();
}

void TableScan::insert() {
    d_current_slot = d_rp->insert_after(d_current_slot);
    while (d_current_slot < 0) {
        if (at_last_block()) {
            move_to_new_block();
        } else {
            move_to_block(d_rp->block().number() + 1);
        }
        d_current_slot = d_rp->insert_after(d_current_slot);
    }
}

int TableScan::get_int(std::string_view field) {
    return d_rp->get_int(d_current_slot, field);
}

std::string TableScan::get_string(std::string_view field) {
    return d_rp->get_string(d_current_slot, field);
}

simpledb::query::Constant TableScan::get_val(std::string_view field) {
    if (d_layout->schema().get_type(field) == FieldType::INT) {
        return simpledb::query::Constant(get_int(field));
    } else if (d_layout->schema().get_type(field) == FieldType::STRING) {
        return simpledb::query::Constant(get_string(field));
    }
    throw std::runtime_error("Invalid field type");
}

void TableScan::set_int(std::string_view field, int value) {
    d_rp->set_int(d_current_slot, field, value);
}

void TableScan::set_string(std::string_view field, std::string_view value) {
    d_rp->set_string(d_current_slot, field, value);
}

void TableScan::set_val(std::string_view field, simpledb::query::Constant value) {
    if (d_layout->schema().get_type(field) == FieldType::INT) {
        set_int(field, value.as_int());
    } else if (d_layout->schema().get_type(field) == FieldType::STRING) {
        set_string(field, value.as_string());
    }
}

RID TableScan::current_rid() {
    return RID(d_rp->block().number(), d_current_slot);
}

void TableScan::delete_record() {
    d_rp->delete_slot(d_current_slot);
}

void TableScan::move_to_new_block() {
    close();
    simpledb::file::BlockId blk = d_tx->append(d_filename);
    d_rp = std::make_shared<RecordPage>(d_tx, blk, *d_layout);
    d_rp->format();
    d_current_slot = -1;
}

void TableScan::move_to_block(int block_number) {
    close();
    simpledb::file::BlockId blk = simpledb::file::BlockId(d_filename, block_number);
    d_rp = std::make_shared<RecordPage>(d_tx, blk, *d_layout);
    d_current_slot = -1;
}

bool TableScan::at_last_block() {
    return d_rp->block().number() == d_tx->size(d_filename) - 1;
}

} // namespace simpledb::record
