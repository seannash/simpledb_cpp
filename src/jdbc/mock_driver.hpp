#pragma once

#include "driver.hpp"

namespace simpledb::jdbc {

class MockDriver : public Driver {
public:
    std::unique_ptr<Connection> connect(std::string_view url, Properties& props) override;
};

} // namespace simpledb::jdbc 