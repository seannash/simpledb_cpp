#pragma once

#include <memory>
#include <string_view>
#include "connection.hpp"
#include "properties.hpp"

namespace simpledb::jdbc {

class Driver {
public:
    virtual ~Driver() = default;
    virtual std::unique_ptr<Connection> connect(std::string_view url, Properties& props) = 0;
};

} // namespace simpledb::jdbc 