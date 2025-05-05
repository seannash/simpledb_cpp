
#include "lexer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>


using namespace simpledb::parse;

TEST_CASE("Lexer") {
    Lexer lexer("SELECT name FROM students WHERE age > 18;");
    REQUIRE(lexer.matchKeyword("select"));
    lexer.eatKeyword("select");
    REQUIRE(lexer.matchId());
    lexer.eatId();
    REQUIRE(lexer.matchKeyword("from"));
    lexer.eatKeyword("from");
    REQUIRE(lexer.matchId());
    lexer.eatId();;
    REQUIRE(lexer.matchKeyword("where"));
    lexer.eatKeyword("where");
    REQUIRE(lexer.matchId());
    lexer.eatId();
    REQUIRE(lexer.matchDelim('>'));
    lexer.eatDelim('>');
    REQUIRE(lexer.matchIntConstant());
    lexer.eatIntConstant();
    REQUIRE(lexer.matchDelim(';'));
    lexer.eatDelim(';');
}


TEST_CASE("Insert Statement") {
    Lexer lexer("INSERT INTO STUDENT(NAME) VALUES('HI');");
    REQUIRE(lexer.matchKeyword("insert"));
    lexer.eatKeyword("insert");
    REQUIRE(lexer.matchKeyword("into"));
    lexer.eatKeyword("into");
    REQUIRE(lexer.matchId());
    lexer.eatId();
    REQUIRE(lexer.matchDelim('('));
    lexer.eatDelim('(');
    REQUIRE(lexer.matchId());
    lexer.eatId();
    REQUIRE(lexer.matchDelim(')'));
    lexer.eatDelim(')');
    REQUIRE(lexer.matchKeyword("values"));
    lexer.eatKeyword("values");
    REQUIRE(lexer.matchDelim('('));
    lexer.eatDelim('(');
    REQUIRE(lexer.matchStringConstant());
    lexer.eatStringConstant();
    REQUIRE(lexer.matchDelim(')'));
    lexer.eatDelim(')');
    REQUIRE(lexer.matchDelim(';'));
    lexer.eatDelim(';');
}