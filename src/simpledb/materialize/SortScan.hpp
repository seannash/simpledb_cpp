#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/materialize/RecordComparator.hpp"
#include "simpledb/materialize/TempTable.hpp"

#include <vector>
#include <memory>

namespace simpledb::materialize {

class SortScan : public simpledb::query::Scan {
public:
    SortScan(std::vector<std::shared_ptr<TempTable>> runs,
             RecordComparator comp);

    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    simpledb::query::Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;
    void close() override;
    void savePosition();
    void restorePosition();

private:
    std::shared_ptr<simpledb::query::UpdateScan> d_s1;
    std::shared_ptr<simpledb::query::UpdateScan> d_s2;
    std::shared_ptr<simpledb::query::UpdateScan> d_currentscan;
    RecordComparator d_comp;
    bool d_hasmore1, d_hasmore2;
    std::vector<simpledb::record::RID> d_savedpositions;
};

} // namespace simpledb::materialize 