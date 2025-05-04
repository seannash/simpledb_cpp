#include "simpledb/jdbc/embedded/embedded_driver.hpp"
#include "jdbc/connection.hpp"
#include "jdbc/statement.hpp"
#include "jdbc/result_set.hpp"
#include "jdbc/metadata.hpp"
#include "jdbc/sql_exception.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>

void doQuery(::jdbc::Statement* stmt, const std::string& cmd) {
    try {
        auto rs = stmt->executeQuery(cmd);
        auto& md = rs->getMetaData();
        int numcols = md.getColumnCount();
        int totalwidth = 0;

        // print header
        for(int i=1; i<=numcols; i++) {
            std::string fldname = md.getColumnName(i);
            int width = md.getColumnDisplaySize(i);
            totalwidth += width;
            std::cout << std::setw(width) << fldname;
        }
        std::cout << std::endl;
        for(int i=0; i<totalwidth; i++)
            std::cout << "-";
        std::cout << std::endl;

        // print records
        while(rs->next()) {
            for (int i=1; i<=numcols; i++) {
                std::string fldname = md.getColumnName(i);
                auto fldtype = md.getColumnType(i);
                int width = md.getColumnDisplaySize(i);
                if (fldtype == ::jdbc::ColumnType::INT) {
                    int ival = rs->getInt(fldname);
                    std::cout << std::setw(width) << ival;
                }
                else {
                    std::string sval = rs->getString(fldname);
                    std::cout << std::setw(width) << sval;
                }
            }
            std::cout << std::endl;
        }
    }
    catch (const ::jdbc::SQLException& e) {
        std::cout << "SQL Exception: " << e.what() << std::endl;
    }
}

void doUpdate(::jdbc::Statement*stmt, const std::string& cmd) {
    try {
        int howmany = stmt->executeUpdate(cmd);
        std::cout << howmany << " records processed" << std::endl;
    }
    catch (const ::jdbc::SQLException& e) {
        std::cout << "SQL Exception: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string connectStr;
    if (argc == 2) {
        connectStr = argv[1];
    } else {
        std::cout << "Connect> ";
        std::getline(std::cin, connectStr);
    }

    std::unique_ptr<::jdbc::Driver> d;
    d = std::make_unique<simpledb::jdbc::embedded::EmbeddedDriver>();

    try {
        auto conn = d->connect(connectStr, {});
        auto stmt = conn->createStatement();

        std::cout << "\nSQL> ";
        std::string cmd;
        while (std::getline(std::cin, cmd)) {
            // Trim whitespace
            cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
            cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);

            if (cmd.empty()) {
                std::cout << "\nSQL> ";
                continue;
            }

            if (cmd == "exit") {
                break;
            }
            else if (cmd.substr(0, 6) == "select") {
                doQuery(stmt.get(), cmd);
            }
            else {
                doUpdate(stmt.get(), cmd);
            }
            std::cout << "\nSQL> ";
        }
    }
    catch (const ::jdbc::SQLException& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}