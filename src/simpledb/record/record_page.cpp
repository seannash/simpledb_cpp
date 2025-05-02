#include "simpledb/record/record_page.hpp"
#include <stdexcept>

namespace simpledb::record {

namespace {
constexpr int EMPTY_SLOT = 0;
constexpr int USED_SLOT = 1;
}

RecordPage::RecordPage(std::shared_ptr<simpledb::tx::Transaction> tx, simpledb::file::BlockId& blk, Layout layout)
    : d_tx(tx)
    , d_blk(blk)
    , d_layout(layout) {
    d_tx->pin(d_blk);
}

int RecordPage::get_int(int slot, std::string_view field) {
    int pos = offset(slot) + d_layout.offset(field);
    return d_tx->get_int(d_blk, pos);
}

std::string RecordPage::get_string(int slot, std::string_view field) {
    int pos = offset(slot) + d_layout.offset(field);
    return d_tx->get_string(d_blk, pos);
}

void RecordPage::set_int(int slot, std::string_view field, int value) {
    int pos = offset(slot) + d_layout.offset(field);
    std::cout << "Setting int at " << pos << " to " << value << std::endl;
    d_tx->set_int(d_blk, pos, value, true);
}

void RecordPage::set_string(int slot, std::string_view field, std::string_view value) {
    int pos = offset(slot) + d_layout.offset(field);
    d_tx->set_string(d_blk, pos, std::string(value), true);
}

void RecordPage::format() {
    int slot = 0;
    while (is_slot_valid(slot)) {
        std::cout << "Formatting slot " << slot << " " << offset(slot) << std::endl;
        d_tx->set_int(d_blk, offset(slot), EMPTY_SLOT, false);
        auto sch = d_layout.schema();
        for (const auto& field : sch.fields()) {
            int pos = offset(slot) + d_layout.offset(field);
            if (sch.get_type(field) == FieldType::INT) {
                d_tx->set_int(d_blk, pos, 0, false);
            } else {
                d_tx->set_string(d_blk, pos, "", false);
            }
        }
        ++slot;
    }
}

void RecordPage::delete_slot(int slot) {
    set_flag(slot, EMPTY_SLOT);
}

int RecordPage::next_after(int slot) {
    return search_after(slot, USED_SLOT);
}

int RecordPage::insert_after(int slot) {
    slot += 1;
    int new_slot = search_after(slot, EMPTY_SLOT);
    if (new_slot >= 0) {
        set_flag(new_slot, USED_SLOT);
    }
    return new_slot;
}

int RecordPage::search_after(int slot, int flag) {
    slot += 1;
    while (is_slot_valid(slot)) {
        if (d_tx->get_int(d_blk, offset(slot)) == flag) {
            return slot;
        }
        ++slot;
    }
    return -1;
}

void RecordPage::set_flag(int slot, int flag) {
    int pos = offset(slot);
    d_tx->set_int(d_blk, pos, flag, true);
}

bool RecordPage::is_slot_valid(int slot) {
    return offset(slot)>=0 && offset(slot+1) <= d_tx->block_size();
}

int RecordPage::offset(int slot) {
    return slot * d_layout.slot_size();
}

} // namespace simpledb::record
