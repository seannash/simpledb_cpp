#pragma once

#include "simpledb/jdbc/metadata_adapter.hpp"
#include "simpledb/record/schema.hpp"
#include <memory>
#include <string>
#include <vector>

namespace simpledb::jdbc::embedded {

class EmbeddedMetadata : public jdbc::MetadataAdapter {
public:
    explicit EmbeddedMetadata(const record::Schema schema);
    ~EmbeddedMetadata() = default;

    std::size_t getColumnCount() const override;
    std::string getColumnName(int columnIndex) const override;
    ::jdbc::ColumnType getColumnType(int columnIndex) const override;
    std::string getColumnTypeName(int columnIndex) const override;
    int getColumnDisplaySize(int columnIndex) const override;

private:
    const record::Schema d_schema;
};

} // namespace simpledb::jdbc::embedded
