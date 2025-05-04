#pragma once

#include "simpledb/jdbc/driver_adapter.hpp"

namespace simpledb::jdbc::embedded {

class EmbeddedDriver : public simpledb::jdbc::DriverAdapter {
public:
    EmbeddedDriver() = default;
    virtual ~EmbeddedDriver() = default;

    bool acceptsURL(std::string_view url) override;
    std::shared_ptr<::jdbc::Connection> connect(
        const std::string_view url, 
        const std::map<std::string, std::string>& info) override;
};

} // namespace simpledb::jdbc::embedded 