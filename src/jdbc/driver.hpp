#pragma once

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <vector>

#include "connection.hpp"

namespace jdbc {

class Driver {
public:
    virtual ~Driver() = default;

    virtual bool acceptsURL(std::string_view url) = 0;
    virtual std::shared_ptr<Connection> connect(
        const std::string_view url, 
        const std::map<std::string, std::string>& info) = 0;
};

} // namespace jdbc
