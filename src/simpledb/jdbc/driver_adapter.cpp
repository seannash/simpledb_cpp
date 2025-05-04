#include "driver_adapter.hpp"

namespace simpledb::jdbc {

bool DriverAdapter::acceptsURL(std::string_view url) {
    throw ::jdbc::SQLException("operation not implemented");
}

std::shared_ptr<::jdbc::Connection> DriverAdapter::connect(
        const std::string_view url, 
        const std::map<std::string, std::string>& info) {
    throw ::jdbc::SQLException("operation not implemented");
}

} // namespace simpledb::jdbc
