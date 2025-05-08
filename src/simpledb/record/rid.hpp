#pragma once

namespace simpledb::record {

class RID {
public:
    RID() = default;
    RID(int block_number, int slot);
    int block_number() const;
    int slot() const;

    bool operator==(const RID& other) const = default;

private:
    int d_block_number;
    int d_slot;
};

} // namespace simpledb::record
