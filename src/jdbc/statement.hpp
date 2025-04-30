#pragma once

#include <memory>
#include <string>
#include "resultset.hpp"

namespace simpledb::jdbc {

class Statement {
public:
    virtual ~Statement() = default;
    virtual std::unique_ptr<ResultSet> executeQuery(const std::string& sql) = 0;
    virtual int executeUpdate(const std::string& sql) = 0;
    virtual void close() = 0;
};

} // namespace simpledb::jdbc 