#pragma once

#include <memory>

#include "connection.hpp"

namespace simpledb::jdbc {

class DataSource {
public:
    virtual ~DataSource() = default;
    virtual std::unique_ptr<Connection> getConnection() = 0;
};

} // namespace simpledb::jdbc