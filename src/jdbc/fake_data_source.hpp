#pragma once

#include "data_source.h"
#include "connection.hpp"
#include "mock_driver.hpp"
#include "properties.hpp"
#include <memory>

namespace simpledb::jdbc {

class FakeDataSource : public DataSource {
public:
    FakeDataSource();
    std::unique_ptr<Connection> getConnection() override;

    void setDatabase(std::string_view databaseParameter);

private:
    MockDriver d_driver;
    std::string d_database;
    Properties d_properties;
};

} // namespace simpledb::jdbc