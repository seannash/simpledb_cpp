
#pragma once

#include <string_view>
#include <vector>
#include <variant>

#include "simpledb/parser/lexer.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/query/expression.hpp"
#include "simpledb/query/term.hpp"
#include "simpledb/query/predicate.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/parser/insert_data.hpp"
#include "simpledb/parser/modify_data.hpp"
#include "simpledb/parser/create_table_data.hpp"
#include "simpledb/parser/delete_data.hpp"
#include "simpledb/parser/create_index_data.hpp"
#include "simpledb/parser/query_data.hpp"
#include "simpledb/parser/create_view_data.hpp"

namespace simpledb::parse {

class Parser {
public:
    using query_t = std::variant<InsertData, ModifyData, DeleteData, CreateTableData, CreateViewData, CreateIndexData>;

    Parser(std::string_view line);

    std::string parse_field();
    std::shared_ptr<simpledb::query::Constant> parse_constant();
    std::shared_ptr<simpledb::query::Expression> parse_expression();
    std::shared_ptr<simpledb::query::Term> parse_term();
    std::shared_ptr<simpledb::query::Predicate> parse_predicate();

    QueryData query();
    query_t parse_update_command();
    query_t parse_create();
    DeleteData parse_delete();
    InsertData parse_insert();
    ModifyData parse_modify();
    CreateTableData parse_create_table();
    CreateViewData parse_create_view();
    CreateIndexData parse_create_index();

private:

    Lexer d_lex;

    std::vector<std::string> parse_select_list();
    std::vector<std::string> parse_table_list();
    std::vector<std::string> parse_field_list();
    std::vector<simpledb::query::Constant> parse_const_list();
    simpledb::record::Schema parse_field_defs();
    simpledb::record::Schema parse_field_def();
    simpledb::record::Schema field_type(std::string_view field_name);

};

} // namespace simpledb::parse