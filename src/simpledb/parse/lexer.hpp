#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <stdexcept>

namespace simpledb::parse {

class BadSyntaxException : public std::runtime_error {
public:
    BadSyntaxException() : std::runtime_error("Bad syntax") {}
};

class Lexer {
public:
    explicit Lexer(std::string_view s);
    
    // Methods to check the status of the current token
    bool matchDelim(char d) const;
    bool matchIntConstant() const;
    bool matchStringConstant() const;
    bool matchKeyword(std::string_view w) const;
    bool matchId() const;
    
    // Methods to "eat" the current token
    void eatDelim(char d);
    int eatIntConstant();
    std::string eatStringConstant();
    void eatKeyword(std::string_view w);
    std::string eatId();

int d_ttype;  // Current token type
    int d_nval;  // Numeric value
    std::string d_sval;  // String value

private:
    void nextToken();
    bool is_keyword(const std::string& s) const;
    
    std::istringstream d_stream;
    std::unordered_set<std::string> d_keywords;
    
};

} // namespace simpledb::parse