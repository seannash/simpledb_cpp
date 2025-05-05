#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "simpledb/file/block_id.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/query/constant.hpp"

namespace simpledb::index::btree {

/**
 * B-tree directory and leaf pages have many commonalities:
 * in particular, their records are stored in sorted order, 
 * and pages split when full.
 * A BTPage object contains this common functionality.
 */
class BTPage {
public:

    BTPage(std::shared_ptr<simpledb::tx::Transaction> tx,
           simpledb::file::BlockId currentblk,
           std::shared_ptr<simpledb::record::Layout> layout);


    int findSlotBefore(const simpledb::query::Constant& searchkey);

    void close();
    bool isFull() const;
    simpledb::file::BlockId split(int splitpos, int flag);
    simpledb::query::Constant getDataVal(int slot) const;
    int getFlag() const;
    void setFlag(int val);
    simpledb::file::BlockId appendNew(int flag);
    void format(const simpledb::file::BlockId& blk, int flag);
    // Methods called only by BTreeDir
    int getChildNum(int slot) const;
    void insertDir(int slot, const simpledb::query::Constant& val, int blknum);
    // Methods called only by BTreeLeaf
    simpledb::record::RID getDataRid(int slot) const;
    void insertLeaf(int slot, const simpledb::query::Constant& val, const simpledb::record::RID& rid);
    void delete_record(int slot);

    int getNumRecs() const;

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    simpledb::file::BlockId d_currentblk;
    std::shared_ptr<simpledb::record::Layout> d_layout;

    int getInt(int slot, std::string_view fldname) const;
    std::string getString(int slot, std::string_view fldname) const;
    simpledb::query::Constant getVal(int slot, std::string_view fldname) const;
    void setInt(int slot, std::string_view fldname, int val);
    void setString(int slot, std::string_view fldname, std::string_view val);
    void setVal(int slot, std::string_view fldname, const simpledb::query::Constant& val);
    void setNumRecs(int n);
    void insert(int slot);
    void copyRecord(int from, int to);
    void transferRecs(int slot, BTPage& dest);
    int fldpos(int slot, std::string_view fldname) const;
    int slotpos(int slot) const;
    void makeDefaultRecord(const simpledb::file::BlockId& blk, int pos);
};

} // namespace simpledb::index::btree