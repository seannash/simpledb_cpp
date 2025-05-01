#include "mock_driver.hpp"

#include "connection.hpp"
#include "sql_exception.hpp"
#include "fake_internal_connection.hpp"
#include <memory>

namespace simpledb::jdbc {

MockDriver::MockDriver()
    : d_internalConnection(std::make_shared<FakeInternalConnection>())
{}

std::unique_ptr<Connection> MockDriver::connect(std::string_view url, Properties& props) {
    if (url == "jdbc:mock:test") {
        return std::make_unique<Connection>(d_internalConnection);
    }
    throw SQLException("Invalid URL");
}

} // namespace simpledb::jdbc 