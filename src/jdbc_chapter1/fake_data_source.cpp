#include "fake_data_source.hpp"
#include "connection.hpp"
#include "fake_internal_connection.hpp"
#include "properties.hpp"
#include <memory>
#include <sstream>

namespace simpledb::jdbc {

FakeDataSource::FakeDataSource()
: d_driver()
, d_database()
, d_properties()
{
}

void FakeDataSource::setDatabase(std::string_view databaseParameter) {
    d_database = databaseParameter;
}

std::unique_ptr<Connection> FakeDataSource::getConnection() {
    std::stringstream ss;
    ss << "jdbc:simpledb:" << d_database;
    return d_driver.getConnection(ss.str(), d_properties);
}

} // namespace simpledb::jdbc

