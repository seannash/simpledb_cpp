#pragma once

namespace simpledb::record {

class RID {
public:
    RID(int block_number, int slot);
    int block_number() const;
    int slot() const;

private:
    int d_block_number;
    int d_slot;
};

} // namespace simpledb::record
