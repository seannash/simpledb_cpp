#pragma once

#include "simpledb/query/constant.hpp"

#include <string>
#include <any>

namespace simpledb::query {

class Scan {
public:

    virtual ~Scan() = default;

    virtual void beforeFirst() = 0;
    virtual bool next() = 0;
    virtual int getInt(std::string_view field) = 0;
    virtual std::string getString(std::string_view field) = 0;
    virtual Constant getVal(std::string_view field) = 0;
    virtual bool hasField(std::string_view field) = 0;
    virtual void close() = 0;    

};

} // namespace simpledb::query
