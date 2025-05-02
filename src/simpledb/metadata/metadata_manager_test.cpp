#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/server/simpledb.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/tx/transaction.hpp"

#include <iostream>
#include <random>
#include <map>

using namespace simpledb::server;
using namespace simpledb::metadata;
using namespace simpledb::record;
using namespace simpledb::tx;

int main() {
    SimpleDB db("metadatamgrtest", 400, 8);
    auto mdm = db.metadata_manager();
   
    return 0;
}
