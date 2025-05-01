#include "driver_manager.hpp"
#include "sql_exception.hpp"
#include "connection.hpp"
#include "properties.hpp"

namespace simpledb::jdbc {

void DriverManager::registerDriver(std::unique_ptr<Driver> driver) {
    d_drivers.push_back(std::move(driver));
}

std::unique_ptr<Connection> DriverManager::getConnection(std::string_view url, Properties& props) {
    for (const auto& driver : d_drivers) {
        if (driver->supports(url)) {
            return driver->getConnection(url, props);
        }
    }
    throw SQLException("No Driver found for url: " + std::string(url));
}

bool DriverManager::supports(std::string_view url) {
    for (const auto& driver : d_drivers) {
        if (driver->supports(url)) {
            return true;
        }
    }
    return false;
}

} // namespace simpledb::jdbc 