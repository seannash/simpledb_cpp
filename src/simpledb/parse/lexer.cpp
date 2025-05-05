#include "simpledb/parse/lexer.hpp"
#include <cctype>
#include <algorithm>

#include <iostream>

namespace simpledb::parse {

namespace {

const char* const KEYWORDS[] = {
    "select", "from", "where", "and",
    "insert", "into", "values", "delete", "update", "set",
    "create", "table", "int", "varchar", "view", "as", "index", "on"
};

enum TokenType {
    TT_EOF = -1,
    TT_DELIM = 0,
    TT_NUMBER = 1,
    TT_STRING = 2,
    TT_KEYWORD = 3,
    TT_ID = 4,
    TT_QUOTE = '\'',
    TT_COMMA = ',',
    TT_LPAREN = '(',
    TT_RPAREN = ')',
    TT_EQUAL = '=',
    TT_LT = '<',
    TT_GT = '>'
};

}

Lexer::Lexer(std::string_view s)
    : d_stream(std::string(s))
    , d_ttype(0)
    , d_nval(0)
{
    d_keywords.insert(KEYWORDS, KEYWORDS + sizeof(KEYWORDS) / sizeof(KEYWORDS[0]));
    nextToken();
}

bool Lexer::matchDelim(char d) const {
    return d == d_ttype;
}

bool Lexer::matchIntConstant() const {
    return d_ttype == TT_NUMBER;
}

bool Lexer::matchStringConstant() const {
    return d_ttype == TT_STRING;
}

bool Lexer::matchKeyword(std::string_view w) const {
    if (d_ttype != TT_KEYWORD) {
        return false;
    }
    std::string word(w);
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word == d_sval;
}

bool Lexer::matchId() const {
    if (d_ttype != TT_ID) {
        return false;
    }
    std::string word = d_sval;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    return d_keywords.find(word) == d_keywords.end();
}

void Lexer::eatDelim(char d) {
    if (!matchDelim(d)) {
        throw BadSyntaxException();
    }
    nextToken();
}

int Lexer::eatIntConstant() {
    if (!matchIntConstant()) {
        throw BadSyntaxException();
    }
    int i = d_nval;
    nextToken();
    return i;
}

std::string Lexer::eatStringConstant() {
    if (!matchStringConstant()) {
        throw BadSyntaxException();
    }
    std::string s = d_sval;
    nextToken();
    return s;
}

void Lexer::eatKeyword(std::string_view w) {
    if (d_ttype != TT_KEYWORD || d_sval != w) {
        throw BadSyntaxException();
    }
    nextToken();
}

std::string Lexer::eatId() {
    if (!matchId()) {
        throw BadSyntaxException();
    }
    std::string s = d_sval;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    nextToken();
    return s;
}

void Lexer::nextToken() {
    char c;
    d_sval.clear();
    
    // Skip whitespace
    while (d_stream.get(c) && std::isspace(c));
    
    if (d_stream.eof()) {
        d_ttype = TT_EOF;
        return;
    }
    
    if (c == '\'') {
        d_ttype = TT_STRING;
        while (d_stream.get(c) && c != '\'') {
            d_sval += c;
        }
        if (d_stream.eof()) {
            throw BadSyntaxException();
        }
        return;
    }
    // Handle delimiters
    if (c == '\'' || c == ',' || c == '(' || c == ')' || c == '=' || c == '<' || c == '>' || c == ';') {
        d_ttype = c;
        return;
    }
    
    // Handle numbers
    if (std::isdigit(c)) {
        d_stream.unget();
        d_stream >> d_nval;
        d_ttype = TT_NUMBER;
        return;
    }
    
    // Handle words and identifiers
    if (std::isalpha(c) || c == '_') {
        d_sval = c;
        while (d_stream.get(c) && (std::isalnum(c) || c == '_')) {
            d_sval += c;
        }
        if (!d_stream.eof()) {
            d_stream.unget();
        }
        std::string keyword = d_sval;
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
        if (d_keywords.contains(keyword)) {
            d_sval = keyword;
            d_ttype = TT_KEYWORD;
        } else {
            d_ttype = TT_ID;
        }
        return;
    }

    throw BadSyntaxException();
}

bool Lexer::is_keyword(const std::string& s) const {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return d_keywords.contains(lower);
}

} // namespace simpledb::parse 