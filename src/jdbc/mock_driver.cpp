#include "mock_driver.hpp"

#include "connection.hpp"
#include "sql_exception.hpp"

#include <memory>

namespace simpledb::jdbc {

std::unique_ptr<Connection> MockDriver::connect(std::string_view url, Properties& props) {
    if (url == "jdbc:mock:test") {
        return std::make_unique<Connection>();
    }
    throw SQLException("Invalid URL");
}

} // namespace simpledb::jdbc 