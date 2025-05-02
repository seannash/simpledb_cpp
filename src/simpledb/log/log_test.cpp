#include <iostream>
#include <memory>

#include "simpledb/file/file_manager.hpp"
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/page.hpp"

std::vector<char> create_log_record(const std::string& rec_name, std::int32_t rec_num) {
    std::vector<char> rec;
    int string_size = rec_name.size();
    rec.insert(rec.end(), rec_name.begin(), rec_name.end());
    rec.insert(rec.end(), &rec_num, &rec_num + sizeof(int));
    return rec;
}

void create_records(std::shared_ptr<simpledb::log::LogManager> log_manager, int start, int end) {
    std::cout << "Creating records " << start << " to " << end << std::endl;
    for (int i = start; i < end; i++) {
        std::string rec_name = "record " + std::to_string(i);
        auto rec = create_log_record(rec_name, i+100);
        int lsn = log_manager->append(rec);
        std::cout << "LSN: " << lsn << std::endl;
    }
}

void print_log_record(const std::span<char>& rec) {
    if (rec.size() == 0) {
        std::cout << "Empty record" << std::endl;
        return;
    }
    std::string message(rec.begin(), rec.end()-sizeof(int));
    int rec_num = 0;
    std::copy(rec.end()-sizeof(int), rec.end(), &rec_num);
    std::cout << "Record (" << rec.size() <<"):" << message << " " << rec_num << std::endl;
    
}

void print_log_records(std::shared_ptr<simpledb::log::LogManager> log_manager) {
    std::cout << "Printing log records" << std::endl;
    auto log_iterator = log_manager->iterator();
    int cnt = 0;
    while (log_iterator.hasNext()) {
        auto rec = log_iterator.next();
        print_log_record(rec);
        cnt++;
    }
    std::cout << "Total records: " << cnt << std::endl;
}


int main() {
    auto file_manager = std::make_shared<simpledb::file::FileManager>("tmp", 4096);
    auto log_manager = std::make_shared<simpledb::log::LogManager>(file_manager, "log.txt");
    create_records(log_manager, 1, 35);
    log_manager->flush();
    print_log_records(log_manager);
}



