#include "simpledb/index/btree/BTPage.hpp"
#include <cstdint>

namespace simpledb::index::btree {

BTPage::BTPage(std::shared_ptr<simpledb::tx::Transaction> tx,
               simpledb::file::BlockId currentblk,
               std::shared_ptr<simpledb::record::Layout> layout)
    : d_tx(std::move(tx))
    , d_currentblk(std::move(currentblk))
    , d_layout(std::move(layout))
{
    d_tx->pin(d_currentblk);
}

int BTPage::findSlotBefore(const simpledb::query::Constant& searchkey)
{
    int slot = 0;
    while (slot < getNumRecs() && getDataVal(slot) < searchkey) {
        slot++;
    }
    return slot - 1;
}

void BTPage::close()
{
    // FIXME: Make d_currentblk a pointer or std::optional?
    if (d_currentblk.number() != -1) { // Using negative
        d_tx->unpin(d_currentblk);
    }
    d_currentblk = simpledb::file::BlockId("", -1);
}

bool BTPage::isFull() const
{
    return slotpos(getNumRecs() + 1) >= d_tx->block_size();
}

simpledb::file::BlockId BTPage::split(int splitpos, int flag)
{
    auto newblk = appendNew(flag);
    BTPage newpage(d_tx, newblk, d_layout);
    transferRecs(splitpos, newpage);
    newpage.setFlag(flag);
    newpage.close();
    return newblk;
}

simpledb::query::Constant BTPage::getDataVal(int slot) const
{
    return getVal(slot, "dataval");
}

int BTPage::getFlag() const
{
    return d_tx->get_int(d_currentblk, 0);
}

void BTPage::setFlag(int val)
{
    d_tx->set_int(d_currentblk, 0, val, true);
}

simpledb::file::BlockId BTPage::appendNew(int flag)
{
    auto blk = d_tx->append(d_currentblk.fileName());
    d_tx->pin(blk);
    format(blk, flag);
    return blk;
}

void BTPage::format(const simpledb::file::BlockId& blk, int flag)
{
    d_tx->set_int(blk, 0, flag, false);
    d_tx->set_int(blk, sizeof(int), 0, false);  // #records = 0
    int recsize = d_layout->slot_size();
    for (int pos = 2 * sizeof(int); pos + recsize <= d_tx->block_size(); pos += recsize) {
        makeDefaultRecord(blk, pos);
    }
}

void BTPage::makeDefaultRecord(const simpledb::file::BlockId& blk, int pos)
{
    for (const auto& fldname : d_layout->schema().fields()) {
        int offset = d_layout->offset(fldname);
        if (d_layout->schema().get_type(fldname) == jdbc::ColumnType::INT) {
            d_tx->set_int(blk, pos + offset, 0, false);
        } else {
            d_tx->set_string(blk, pos + offset, "", false);
        }
    }
}

int BTPage::getChildNum(int slot) const
{
    return getInt(slot, "block");
}

void BTPage::insertDir(int slot, const simpledb::query::Constant& val, int blknum)
{
    insert(slot);
    setVal(slot, "dataval", val);
    setInt(slot, "block", blknum);
}

simpledb::record::RID BTPage::getDataRid(int slot) const
{
    int blknum = getInt(slot, "block");
    int id = getInt(slot, "id");
    return simpledb::record::RID(blknum, id);
}

void BTPage::insertLeaf(int slot, const simpledb::query::Constant& val, const simpledb::record::RID& rid)
{
    insert(slot);
    setVal(slot, "dataval", val);
    setInt(slot, "block", rid.block_number());
    setInt(slot, "id", rid.slot());
}

void BTPage::delete_record(int slot)
{
    for (int i = slot + 1; i < getNumRecs(); i++) {
        copyRecord(i, i - 1);
    }
    setNumRecs(getNumRecs() - 1);
}

int BTPage::getNumRecs() const
{
    return d_tx->get_int(d_currentblk, sizeof(int));
}

// Private helper methods
int BTPage::getInt(int slot, std::string_view fldname) const
{
    int pos = fldpos(slot, fldname);
    return d_tx->get_int(d_currentblk, pos);
}

std::string BTPage::getString(int slot, std::string_view fldname) const
{
    int pos = fldpos(slot, fldname);
    return d_tx->get_string(d_currentblk, pos);
}

simpledb::query::Constant BTPage::getVal(int slot, std::string_view fldname) const
{
    auto type = d_layout->schema().get_type(fldname);
    if (type == jdbc::ColumnType::INT) {
        return simpledb::query::Constant(getInt(slot, fldname));
    } else {
        return simpledb::query::Constant(getString(slot, fldname));
    }
}

void BTPage::setInt(int slot, std::string_view fldname, int val)
{
    int pos = fldpos(slot, fldname);
    d_tx->set_int(d_currentblk, pos, val, true);
}

void BTPage::setString(int slot, std::string_view fldname, std::string_view val)
{
    int pos = fldpos(slot, fldname);
    // FIXME: Extra conversion
    std::string str_val(val);
    d_tx->set_string(d_currentblk, pos, str_val, true);
}

void BTPage::setVal(int slot, std::string_view fldname, const simpledb::query::Constant& val)
{
    auto type = d_layout->schema().get_type(fldname);
    if (type == jdbc::ColumnType::INT) {
        setInt(slot, fldname, val.as_int());
    } else {
        setString(slot, fldname, val.as_string());
    }
}

void BTPage::setNumRecs(int n)
{
    d_tx->set_int(d_currentblk, sizeof(int), n, true);
}

void BTPage::insert(int slot)
{
    for (int i = getNumRecs(); i > slot; i--) {
        copyRecord(i - 1, i);
    }
    setNumRecs(getNumRecs() + 1);
}

void BTPage::copyRecord(int from, int to)
{
    auto schema = d_layout->schema();
    for (const auto& fldname : schema.fields()) {
        setVal(to, fldname, getVal(from, fldname));
    }
}

void BTPage::transferRecs(int slot, BTPage& dest)
{
    int destslot = 0;
    while (slot < getNumRecs()) {
        dest.insert(destslot);
        auto schema = d_layout->schema();
        for (const auto& fldname : schema.fields()) {
            dest.setVal(destslot, fldname, getVal(slot, fldname));
        }
        delete_record(slot);
        destslot++;
    }
}

int BTPage::fldpos(int slot, std::string_view fldname) const
{
    int offset = d_layout->offset(fldname);
    return slotpos(slot) + offset;
}

int BTPage::slotpos(int slot) const
{
    int slotsize = d_layout->slot_size();
    return sizeof(int) + sizeof(int) + (slot * slotsize);
}

} // namespace simpledb::index::btree 