#include "simpledb/record/rid.hpp"

namespace simpledb::record {

RID::RID(int block_number, int slot)
    : d_block_number(block_number)
    , d_slot(slot) {
}

int RID::block_number() const {
    return d_block_number;
}

int RID::slot() const {
    return d_slot;
}

} // namespace simpledb::record
