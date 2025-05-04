#pragma once

#include <memory>
#include <string>

#include "metadata.hpp"

namespace jdbc {

class ResultSet {
public:

    ~ResultSet() = default;
    virtual bool next() = 0;
    virtual std::string getString(const std::string& columnLabel) = 0;
    virtual std::string getString(int columnIndex) = 0;
    virtual int getInt(const std::string& columnLabel) = 0;
    virtual int getInt(int columnIndex) = 0;
    virtual void close() = 0;
    virtual const MetaData& getMetaData() = 0;
};

} // namespace jdbc 