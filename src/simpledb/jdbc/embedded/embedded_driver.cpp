#include "embedded_driver.hpp"
#include "simpledb/server/simpledb.hpp"
#include "simpledb/jdbc/embedded/embedded_connection.hpp"
#include "simpledb/jdbc/connection_adapter.hpp"
#include <string_view>

namespace simpledb::jdbc::embedded {

bool EmbeddedDriver::acceptsURL(std::string_view url) {
    return true;
}

std::shared_ptr<::jdbc::Connection> EmbeddedDriver::connect(const std::string_view url, const std::map<std::string, std::string>& info) {
    auto db = std::make_unique<server::SimpleDB>(url);
    return std::make_shared<simpledb::jdbc::embedded::EmbeddedConnection>(std::move(db));
}

} // namespace simpledb::jdbc::embedded
