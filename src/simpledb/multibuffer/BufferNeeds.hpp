#pragma once

namespace simpledb::multibuffer {

class BufferNeeds {
public:

    BufferNeeds() = delete;

    static int bestRoot(int available, int size);
    static int bestFactor(int available, int size);

};

} // namespace simpledb::multibuffer
