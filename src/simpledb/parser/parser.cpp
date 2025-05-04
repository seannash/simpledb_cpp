#include "simpledb/parser/parser.hpp"
#include <stdexcept>

#include <iostream>

namespace simpledb::parse {

Parser::Parser(std::string_view line)
    : d_lex(line) {
}

std::string Parser::parse_field() {
    return d_lex.eatId();
}

std::shared_ptr<simpledb::query::Constant> Parser::parse_constant() {
    if (d_lex.matchIntConstant()) {
        return std::make_shared<simpledb::query::Constant>(d_lex.eatIntConstant());
    }
    return std::make_shared<simpledb::query::Constant>(d_lex.eatStringConstant());
}

std::shared_ptr<simpledb::query::Expression> Parser::parse_expression() {
    if (d_lex.matchId()) {
        return std::make_shared<simpledb::query::Expression>(d_lex.eatId());
    }
    return std::make_shared<simpledb::query::Expression>(*parse_constant());
}

std::shared_ptr<simpledb::query::Term> Parser::parse_term() {
    auto lhs = parse_expression();
    d_lex.eatDelim('=');
    auto rhs = parse_expression();
    return std::make_shared<simpledb::query::Term>(lhs, rhs);
}

std::shared_ptr<simpledb::query::Predicate> Parser::parse_predicate() {
    auto pred = std::make_shared<simpledb::query::Predicate>(parse_term());
    if (d_lex.matchKeyword("and")) {
        d_lex.eatKeyword("and");
        pred->conjoin_with(std::make_shared<simpledb::query::Predicate>(parse_term()));
    }
    return pred;
}

QueryData Parser::query() {
    d_lex.eatKeyword("select");
    auto select_list = parse_select_list();
    d_lex.eatKeyword("from");
    auto table_list = parse_table_list();
    auto pred = std::make_shared<simpledb::query::Predicate>();
    if (d_lex.matchKeyword("where")) {
        d_lex.eatKeyword("where");
        pred = parse_predicate();
    }
    return QueryData(select_list, table_list, pred);
}

Parser::query_t Parser::parse_update_command() {
    if (d_lex.matchKeyword("insert")) {
        return parse_insert();
    } else if (d_lex.matchKeyword("delete")) {
        return parse_delete();
    } else if (d_lex.matchKeyword("update")) {
        return parse_modify();
    }
    return parse_create();
}

Parser::query_t Parser::parse_create() {
    d_lex.eatKeyword("create");
    if (d_lex.matchKeyword("table")) {
        return parse_create_table();
    } else if (d_lex.matchKeyword("view")) {
        return parse_create_view();
    } else if (d_lex.matchKeyword("index")) {
        return parse_create_index();
    }
    throw BadSyntaxException();
}

DeleteData Parser::parse_delete() {
    d_lex.eatKeyword("delete");
    d_lex.eatKeyword("from");
    auto table_name = d_lex.eatId();
    std::shared_ptr<simpledb::query::Predicate> pred {};
    if (d_lex.matchKeyword("where")) {
        d_lex.eatKeyword("where");
        pred = parse_predicate();
    }
    return DeleteData(table_name, pred);
}

InsertData Parser::parse_insert() {
    d_lex.eatKeyword("insert");
    d_lex.eatKeyword("into");
    auto table_name = d_lex.eatId();
    d_lex.eatDelim('(');
    auto fields = parse_field_list();
    d_lex.eatDelim(')');
    d_lex.eatKeyword("values");
    d_lex.eatDelim('(');
    auto values = parse_const_list();
    d_lex.eatDelim(')');
    return InsertData(table_name, fields, values);
}

ModifyData Parser::parse_modify() {
    d_lex.eatKeyword("update");
    auto table_name = d_lex.eatId();
    d_lex.eatKeyword("set");
    auto target_field = parse_field();
    d_lex.eatDelim('=');
    auto new_value = parse_expression();
    std::shared_ptr<simpledb::query::Predicate> predicate {};
    if (d_lex.matchKeyword("where")) {
        d_lex.eatKeyword("where");
        predicate = parse_predicate();
    }
    return ModifyData(table_name, target_field, new_value, predicate);
}

CreateViewData Parser::parse_create_view() {
    d_lex.eatKeyword("view");
    auto view_name = d_lex.eatId();
    d_lex.eatKeyword("as");
    QueryData query_data = query();
    return CreateViewData(view_name, query_data);
}

CreateIndexData Parser::parse_create_index() {
    d_lex.eatKeyword("index");
    auto index_name = d_lex.eatId();
    d_lex.eatKeyword("on");
    auto table_name = d_lex.eatId();
    d_lex.eatDelim('(');
    auto field_name = parse_field();
    d_lex.eatDelim(')');
    return CreateIndexData(index_name, table_name, field_name);
}

CreateTableData Parser::parse_create_table() {
    d_lex.eatKeyword("table");
    auto table_name = d_lex.eatId();
    d_lex.eatDelim('(');
    auto fields = parse_field_defs();
    d_lex.eatDelim(')');
    return CreateTableData(table_name, fields);
}

std::vector<std::string> Parser::parse_select_list() {
    std::vector<std::string> fields;
    fields.push_back(parse_field());
    while (d_lex.matchDelim(',')) {
        d_lex.eatDelim(',');
        fields.push_back(parse_field());
    }
    return fields;
}

std::vector<std::string> Parser::parse_table_list() {
    std::vector<std::string> tables;
    tables.push_back(parse_field());
    while (d_lex.matchDelim(',')) {
        d_lex.eatDelim(',');
        tables.push_back(parse_field());
    }
    return tables;
}

std::vector<std::string> Parser::parse_field_list() {
    std::vector<std::string> fields;
    fields.push_back(parse_field());
    while (d_lex.matchDelim(',')) {
        d_lex.eatDelim(',');
        fields.push_back(parse_field());
    }
    return fields;
}

std::vector<simpledb::query::Constant> Parser::parse_const_list() {
    std::vector<simpledb::query::Constant> constants;
    constants.push_back(*parse_constant());
    while (d_lex.matchDelim(',')) {
        d_lex.eatDelim(',');
        constants.push_back(*parse_constant());
    }
    return constants;
}

simpledb::record::Schema Parser::parse_field_defs() {
    simpledb::record::Schema schema;
    schema.add_all(parse_field_def());
    while (d_lex.matchDelim(',')) {
        d_lex.eatDelim(',');
        schema.add_all(parse_field_defs());
    }
    return schema;
}

simpledb::record::Schema Parser::parse_field_def() {
    auto field_name = parse_field();
    return field_type(field_name);
}

simpledb::record::Schema Parser::field_type(std::string_view field_name) {
    simpledb::record::Schema schema{};
    if (d_lex.matchKeyword("int")) {
        d_lex.eatKeyword("int");
        schema.add_int_field(field_name);
    } else {
        d_lex.eatKeyword("varchar");
        d_lex.eatDelim('(');
        int length = d_lex.eatIntConstant();
        d_lex.eatDelim(')');
        schema.add_string_field(field_name, length);
    }
    return schema;
}

} // namespace simpledb::parse
