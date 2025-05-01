#pragma once

#include <memory>
#include <string_view>

#include "connection.hpp"
#include "properties.hpp"
#include "driver.hpp"

namespace simpledb::jdbc {

class DriverManager: public Driver {
public:
    DriverManager() = default;

    void registerDriver(std::unique_ptr<Driver> driver);
    std::unique_ptr<Connection> getConnection(std::string_view url, Properties& props) override;
    bool supports(std::string_view url) override;
private:
    std::vector<std::unique_ptr<Driver>> d_drivers;
};

} // namespace simpledb::jdbc 