#include "mock_driver.hpp"

#include "connection.hpp"
#include "sql_exception.hpp"
#include "fake_internal_connection.hpp"
#include <memory>

namespace simpledb::jdbc {

MockDriver::MockDriver()
    : d_internalConnection(std::make_shared<FakeInternalConnection>())
{}

std::unique_ptr<Connection> MockDriver::getConnection(std::string_view url, Properties& props) {
    if (url == "jdbc:simpledb:test") {
        return std::make_unique<Connection>(d_internalConnection);
    }
    throw SQLException("Invalid URL");
}

bool MockDriver::supports(std::string_view url) {
    return url == "jdbc:simpledb:test";
}

} // namespace simpledb::jdbc 