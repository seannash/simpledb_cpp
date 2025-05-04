#include "simpledb/tx/transaction.hpp"
#include "simpledb/plan/plan.hpp"
#include "simpledb/plan/planner.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/server/simpledb.hpp"
#include <iostream>
#include <string>
#include <memory>

int main() {
    try {
        // analogous to the driver
        simpledb::server::SimpleDB db("studentdb");

        // analogous to the connection
        auto tx = db.new_tx();
        auto planner = db.planner();
        
        // analogous to the statement
        std::string qry = "select SName, DName "
                "from DEPT, STUDENT "
                "where MajorId = DId";
        auto p = planner->createQueryPlan(qry, tx);
        
        // analogous to the result set
        auto s = p->open();
        
        std::cout << "Name\tMajor" << std::endl;
        while (s->next()) {
            std::string sname = s->get_string("sname"); //SimpleDB stores field names
            std::string dname = s->get_string("dname"); //in lower case
            std::cout << sname << "\t" << dname << std::endl;
        }
        s->close();
        tx->commit();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}