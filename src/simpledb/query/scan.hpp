#pragma once

#include "simpledb/query/constant.hpp"

#include <string>
#include <any>

namespace simpledb::query {

class Scan {
public:

    virtual ~Scan() = default;

    virtual void before_first() = 0;
    virtual bool next() = 0;
    virtual int get_int(std::string_view field) = 0;
    virtual std::string get_string(std::string_view field) = 0;
    virtual Constant get_val(std::string_view field) = 0;
    virtual bool has_field(std::string_view field) = 0;
    virtual void close() = 0;    

};

} // namespace simpledb::query
