#include "simpledb/buffer/buffer_manager.hpp"
#include "simpledb/file/file_manager.hpp"
#include "simpledb/log/log_manager.hpp"

#include <iostream>

int main() {
    auto file_manager = std::make_shared<simpledb::file::FileManager>("test.txt", 4096);
    auto log_manager = std::make_shared<simpledb::log::LogManager>(file_manager, "log.txt");
    auto buffer_manager = std::make_shared<simpledb::buffer::BufferManager>(file_manager, log_manager, 10);

    auto buff1 = buffer_manager->pin(simpledb::file::BlockId("test.txt", 1));
    simpledb::file::Page& p = buff1->contents();
    int n = p.getInt(80);
    std::cout << "The original value is " << n << std::endl;
    p.setInt(80, n+1);
    buff1->set_modified(1, 0);
    std::cout << "The new value is " << n+1 << std::endl;
    buffer_manager->unpin(*buff1);
    auto buff2 = buffer_manager->pin(simpledb::file::BlockId("test.txt", 2));
    auto buff3 = buffer_manager->pin(simpledb::file::BlockId("test.txt", 3));
    auto buff4 = buffer_manager->pin(simpledb::file::BlockId("test.txt", 4));

    buffer_manager->unpin(*buff2);
    buff2 = buffer_manager->pin(simpledb::file::BlockId("test.txt", 1));
    simpledb::file::Page& p2 = buff2->contents();
    std::cout << "The new value is now " << p2.getInt(80) << std::endl;
    p2.setInt(80, 9999);
    buff2->set_modified(1, 0);
    buffer_manager->unpin(*buff2);
}