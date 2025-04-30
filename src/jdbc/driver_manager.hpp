#pragma once

#include <memory>
#include <string_view>

#include "connection.hpp"
#include "properties.hpp"
#include "driver.hpp"

namespace simpledb::jdbc {

class DriverManager {
public:
    static std::unique_ptr<Connection> getConnection(std::string_view url, Properties& props);
};

} // namespace simpledb::jdbc 