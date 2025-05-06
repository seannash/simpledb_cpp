#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"

#include <memory>

namespace simpledb::plan {

class Plan {
public:
    virtual ~Plan() = default;

    virtual std::shared_ptr<query::Scan> open() = 0;
    virtual int blocks_accessed() const = 0;
    virtual int records_output() const = 0;
    virtual int distinct_values(std::string_view field) const = 0;
    virtual const record::Schema& schema() const = 0;
};

} // namespace simpledb::plan