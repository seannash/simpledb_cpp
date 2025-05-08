#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/materialize/AggregateFn.hpp"
#include "simpledb/materialize/GroupValue.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

namespace simpledb::materialize {

class GroupByScan : public simpledb::query::Scan {
public:
    GroupByScan(std::shared_ptr<simpledb::query::Scan> s,
                std::vector<std::string> groupfields,
                std::vector<std::shared_ptr<AggregateFn>> aggfns);

    void before_first() override;
    bool next() override;
    int get_int(std::string_view field) override;
    std::string get_string(std::string_view field) override;
    simpledb::query::Constant get_val(std::string_view field) override;
    bool has_field(std::string_view field) override;
    void close() override;

private:
    std::shared_ptr<simpledb::query::Scan> d_scan;
    std::vector<std::string> d_groupfields;
    std::vector<std::shared_ptr<AggregateFn>> d_aggfns;
    GroupValue d_groupval;
    bool d_more_groups;

    void processGroup();
};

} // namespace simpledb::materialize 