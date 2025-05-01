#include "prepared_statement.hpp"
#include "sql_exception.hpp"

#include <sstream>

namespace simpledb::jdbc {

namespace {

std::vector<std::string> split(std::string str) {
    std::vector<std::string> tokens;
    std::string token;
    char delimiter = '?';
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

}

PreparedStatement::PreparedStatement(std::shared_ptr<InternalConnection> internalConnection, std::string_view sql)
    : d_internalConnection(internalConnection), d_sql(sql)
{
    d_tokens = split(std::string(d_sql));
    d_parameters.resize(d_tokens.size() - 1);
}

void PreparedStatement::setString(int parameterIndex, const std::string& value) {
    if (parameterIndex < 1 || static_cast<size_t>(parameterIndex) > d_parameters.size()) {
        throw SQLException("Invalid parameter index: " + std::to_string(parameterIndex));
    }
    d_parameters[parameterIndex - 1] = value;
}

void PreparedStatement::setInt(int parameterIndex, int value) {
    if (parameterIndex < 1 || static_cast<size_t>(parameterIndex) > d_parameters.size()) {
        throw SQLException("Invalid parameter index: " + std::to_string(parameterIndex));
    }
    d_parameters[parameterIndex - 1] = std::to_string(value);
}

void PreparedStatement::clearParameters() {
    d_parameters.clear();
}

std::unique_ptr<ResultSet> PreparedStatement::executeQuery() {
    std::string full_query = buildSql();
    return d_internalConnection->executeQuery(full_query);
}

int PreparedStatement::executeUpdate() {
    if (!d_internalConnection) {
        throw SQLException("No internal connection available");
    }
    std::string full_query = buildSql();
    return d_internalConnection->executeUpdate(full_query);
}

void PreparedStatement::close() {
    d_internalConnection->close();
}

std::string PreparedStatement::buildSql() const {
    std::stringstream ss {};
    for (int i=0; i<d_tokens.size()-1; ++i) {
        ss << d_tokens[i];
        ss << d_parameters[i];
    }
    ss << d_tokens[d_tokens.size() - 1];
    return ss.str();
}

} // namespace simpledb::jdbc
