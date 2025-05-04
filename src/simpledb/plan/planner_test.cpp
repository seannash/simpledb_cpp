#include "simpledb/server/simpledb.hpp"

#include "simpledb/plan/planner.hpp"

using namespace simpledb;

int main() {
    auto db = std::make_shared<simpledb::server::SimpleDB>("studentdb");
    auto planner = db->planner();
    auto tx = db->new_tx();

    std::cout << "Part -1: Create a table" << std::endl;
    std::string create_table = "create table student (sname varchar(20), gradeyear int, majorid int)";
    planner->executeUpdate(create_table, tx);

    {
    
        std::cout << "Part 0: Add some records" << std::endl;
        std::string insert = "insert into student (sname, gradeyear, majorid) values ('John', 2025, 25)";
        planner->executeUpdate(insert, tx);
        insert = "insert into student (sname, gradeyear, majorid) values ('Jane', 2024, 30)";
        planner->executeUpdate(insert, tx);
        insert = "insert into student (sname, gradeyear, majorid) values ('Jim', 2023, 30)";
        planner->executeUpdate(insert, tx);
    }
    {
        std::cout << "Part 1: Process a query" << std::endl;
        //std::string query = "SELECT sname, gradeyear FROM student";
        std::string query = "SELECT sname, gradeyear FROM student WHERE majorid = 30";
        auto p = planner->createQueryPlan(query, tx);
        auto s = p->open();
        while (s->next()) {
            std::cout << s->get_string("sname") << " " << s->get_int("gradeyear") << std::endl;
        }
        s->close();
    }

    {
        std::cout << "Part 2: Process an update" << std::endl;
        std::string cmd = "delete from student where MajorId = 30";
        int num_affected = planner->executeUpdate(cmd, tx);
        std::cout << "Number of affected records: " << num_affected << std::endl;
    }
    {
        std::cout << "Part 3: Display all records" << std::endl;
        std::string query = "SELECT sname, gradeyear FROM student";
        auto p = planner->createQueryPlan(query, tx);
        auto s = p->open();
        while (s->next()) {
            std::cout << s->get_string("sname") << " " << s->get_int("gradeyear") << std::endl;
        }
        s->close();
    }
}