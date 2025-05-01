#pragma once

#include "driver.hpp"

namespace simpledb::jdbc {

class MockDriver : public Driver {
public:
    MockDriver();
    ~MockDriver() = default;
    std::unique_ptr<Connection> getConnection(std::string_view url, Properties& props) override;
    bool supports(std::string_view url) override;
private:
    std::shared_ptr<InternalConnection> d_internalConnection;
};

} // namespace simpledb::jdbc 