#include "simpledb/metadata/table_manager.hpp"
#include <stdexcept>
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/table_scan.hpp"

namespace simpledb::metadata {

namespace {

constexpr int MAX_SLOTS = 1000;

}

TableManager::TableManager(bool is_new, std::shared_ptr<simpledb::tx::Transaction> tx)
: d_tx(tx) {
    auto tcat_schema = std::make_shared<simpledb::record::Schema>();
    tcat_schema->add_string_field("tblname", MAX_NAME_LEN);
    tcat_schema->add_int_field("slotsize");
    d_tcat_layout = std::make_shared<simpledb::record::Layout>(*tcat_schema);
    
    auto fcat_schema = std::make_shared<simpledb::record::Schema>();
    fcat_schema->add_string_field("tblname", MAX_NAME_LEN);
    fcat_schema->add_string_field("fldname", MAX_NAME_LEN);
    fcat_schema->add_int_field("type");
    fcat_schema->add_int_field("length");
    fcat_schema->add_int_field("offset");
    d_fcat_layout = std::make_shared<simpledb::record::Layout>(*fcat_schema);
    
    if (is_new) {
        create_table("tblcat", tcat_schema, d_tx);
        create_table("fldcat", fcat_schema, d_tx);
    }

}

void TableManager::create_table(std::string_view table_name,
                              std::shared_ptr<simpledb::record::Schema> schema,
                              std::shared_ptr<simpledb::tx::Transaction> tx) {
    
    auto layout = std::make_shared<simpledb::record::Layout>(*schema);
    // Insert one record into tblcat
    simpledb::record::TableScan tcat_scan(tx, "tblcat", d_tcat_layout);
    tcat_scan.insert();
    tcat_scan.set_string("tblname", table_name);
    tcat_scan.set_int("slotsize", layout->slot_size());
    tcat_scan.close();
    
    simpledb::record::TableScan fcat_scan(tx, "fldcat", d_fcat_layout);
    for (const auto& field : schema->fields()) {
        fcat_scan.insert();
        fcat_scan.set_string("tblname", table_name);
        fcat_scan.set_string("fldname", field);
        fcat_scan.set_int("type", (int)schema->get_type(field));
        fcat_scan.set_int("length", schema->get_length(field));
        fcat_scan.set_int("offset", layout->offset(field));
    }
    fcat_scan.close();
}


std::shared_ptr<simpledb::record::Layout> TableManager::get_layout(
    std::string_view table_name,
    std::shared_ptr<simpledb::tx::Transaction> tx) const {
    int size = -1;
    simpledb::record::TableScan tcat_scan(tx, "tblcat", d_tcat_layout);
    while (tcat_scan.next()) {
        if (tcat_scan.get_string("tblname") == table_name) {
            size = tcat_scan.get_int("slotsize");
            break;
        }
    }
    tcat_scan.close();
    if (size == -1) {
        throw std::runtime_error("Table " + std::string(table_name) + " not found");
    }
   auto schema = std::make_shared<simpledb::record::Schema>();
   std::unordered_map<std::string, int> offsets {};
   simpledb::record::TableScan fcat_scan(tx, "fldcat", d_fcat_layout);
   while (fcat_scan.next()) {
      if (fcat_scan.get_string("tblname") == table_name) {
         auto field = fcat_scan.get_string("fldname");
         auto type = (::jdbc::ColumnType)fcat_scan.get_int("type");
         auto length = fcat_scan.get_int("length");
         auto offset = fcat_scan.get_int("offset");
         offsets[field] = offset;
         schema->add_field(field, type, length);
         schema->add_field(fcat_scan.get_string("fldname"),
                           (::jdbc::ColumnType)fcat_scan.get_int("type"),
                           fcat_scan.get_int("length"));
      }
   }
   fcat_scan.close();
   return std::make_shared<simpledb::record::Layout>(*schema, offsets, size);
}

} // namespace simpledb::metadata
