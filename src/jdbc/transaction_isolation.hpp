#pragma once

namespace simpledb::jdbc {

/**
 * @brief Transaction isolation levels as defined in JDBC
 * 
 * These levels control how transactions interact with each other:
 * - READ_UNCOMMITTED: Lowest isolation level, allows dirty reads
 * - READ_COMMITTED: Prevents dirty reads, but allows non-repeatable reads
 * - REPEATABLE_READ: Prevents dirty and non-repeatable reads, but allows phantom reads
 * - SERIALIZABLE: Highest isolation level, prevents all concurrency issues
 */
enum class TransactionIsolation {
    READ_UNCOMMITTED,
    READ_COMMITTED,
    REPEATABLE_READ,
    SERIALIZABLE
};

} // namespace simpledb::jdbc 