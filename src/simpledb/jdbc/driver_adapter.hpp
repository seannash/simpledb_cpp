#pragma once

#include "jdbc/driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/sql_exception.hpp"

namespace simpledb::jdbc {

class DriverAdapter : public ::jdbc::Driver {
public:
    DriverAdapter() = default;
    virtual ~DriverAdapter() = default;

    virtual bool acceptsURL(std::string_view url) override;
    virtual std::shared_ptr<::jdbc::Connection> connect(
        const std::string_view url, 
        const std::map<std::string, std::string>& info) override;
};

} // namespace simpledb::jdbc