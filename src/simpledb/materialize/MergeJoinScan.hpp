#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/materialize/SortScan.hpp"
#include "simpledb/query/constant.hpp"

#include <memory>
#include <string>

namespace simpledb::materialize {


class MergeJoinScan : public simpledb::query::Scan {
public:

    MergeJoinScan(std::shared_ptr<simpledb::query::Scan> s1,
                 std::shared_ptr<SortScan> s2,
                 std::string fldname1,
                 std::string fldname2);


    void close() override;
    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    simpledb::query::Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;

private:
    std::shared_ptr<simpledb::query::Scan> d_s1;
    std::shared_ptr<SortScan> d_s2;
    std::string d_fldname1;
    std::string d_fldname2;
    simpledb::query::Constant d_joinval;
};

} // namespace simpledb::materialize
