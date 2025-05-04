#pragma once

#include <memory>
#include <string_view>
#include "result_set.hpp"

namespace jdbc {

class Statement {
public:
    ~Statement() = default;
    virtual std::unique_ptr<ResultSet> executeQuery(std::string_view sql) = 0;
    virtual int executeUpdate(std::string_view sql) = 0;
    virtual void close() = 0;
};

} // namespace jdbc 