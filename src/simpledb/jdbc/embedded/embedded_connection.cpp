#include "embedded_connection.hpp"
#include "simpledb/jdbc/statement_adapter.hpp"
#include "jdbc/sql_exception.hpp"

#include "simpledb/jdbc/embedded/embedded_statement.hpp"

namespace simpledb::jdbc::embedded {

EmbeddedConnection::EmbeddedConnection(std::unique_ptr<server::SimpleDB> db)
: d_db(std::move(db))
, d_current_tx(d_db->new_tx())
{}

std::unique_ptr<::jdbc::Statement> EmbeddedConnection::createStatement() {
    if (!d_db) {
        throw ::jdbc::SQLException("Connection is closed");
    }
    return std::make_unique<EmbeddedStatement>(this, d_db->planner());
}

void EmbeddedConnection::close() {
    if (d_current_tx) {
        rollback();
    }
    d_db.reset();
}

void EmbeddedConnection::commit() {
    if (!d_db) {
        throw ::jdbc::SQLException("Connection is closed");
    }
    d_current_tx->commit();
    d_current_tx = d_db->new_tx();
}

void EmbeddedConnection::rollback() {
    if (!d_db) {
        throw ::jdbc::SQLException("Connection is closed");
    }
    if (!d_current_tx) {
        throw ::jdbc::SQLException("No transaction is active");
    }
    d_current_tx->rollback();
    d_current_tx = d_db->new_tx();
}

std::shared_ptr<simpledb::tx::Transaction> EmbeddedConnection::get_transaction() const {
    return d_current_tx;
};

} // namespace simpledb::jdbc::embedded 