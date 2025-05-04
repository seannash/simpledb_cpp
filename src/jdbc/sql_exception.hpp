#pragma once

#include <string>
#include <stdexcept>

namespace jdbc {

class SQLException : public std::exception {
public:
    SQLException(const std::string& message);
    const char* what() const noexcept override;
private:
    std::string message;
};

} // namespace jdbc 