#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"

#include <memory>

namespace simpledb::plan {

class Plan {
public:
    virtual ~Plan() = default;

    virtual std::shared_ptr<query::Scan> open() = 0;
    virtual int blocks_accessed() = 0;
    virtual int records_output() = 0;
    virtual int distinct_values(std::string_view field) = 0;
    virtual record::Schema& schema() = 0;
};

} // namespace simpledb::plan