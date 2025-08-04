#include <iostream> 
#include <optional>
#include <vector>

#include "absl/container/btree_set.h"

// Here I will create a char array of size 4096, and use buddy allocation
// to allocate pointers from that array.

// The buddy allocator is a freelist implementation that allocates blocks in
// powers of 2. When some bytes are requested, the allocator allocates a memory
// chunk that is the smallest power of 2 that is greater than the number of
// bytes requested. If no such chunk exist yet, the allocator splits a larger
// chunk in half, and the resulting halves are "buddies".
// When memory is freed, the allocator checks whether the freed chunk's
// "buddy" chunk is also free, and if it is, the buddied chunks are merged into 
// a bigger chunk.

// I'll implement this with log2(4096) = 12 absl::btree_set which allows
// constant-time checking for whether a chunk's buddy is free or not. In
// addition, a absl::btree_set sorts its values, so the next chunk to allocate
// will be the first value in one of the freelist sets.

// Test cases
// 1. Allocate 4096 chunks of size 1
// 2. Allocate 1024 chunks of size 4
// 3. Allocate 512 chunks of size 5 (which rounds up to 8)
// 4. Allocate 1 chunk of size 4096
// 5. Free all allocated pointers, then repeat test cases 1-4

// Future features:
// * Dynamically resize BuddyAllocator instead of failing to allocate pointer.

class BuddyAllocator {
 public:
    char* Allocate(int num_bytes);
    void Free(char* addr);
 private:
    const std::vector<char> buffer;
    // Stores the base pointer of the buffer. The allocator will mostly
    // operate in terms of buffer offset (for easier debug), and will 
    // convert to/from addresses only when interfacing with the client.
    const char* base_ptr_;
    // The hash map at index used_list[i] contains the address of chunks
    // of size 2^i which are in use.
    const std::vector<absl::btree_set<int>> used_list_;
    // The hash map at index free_list[i] contains the address of chunks
    // of size 2^i which are not in use.
    const std::vector<absl::btree_set<int>> free_list_;
};

char* BuddyAllocator::Allocate(int num_bytes) {
    // TODO: Implement.
    return 0;
}

void BuddyAllocator::Free(char* addr) {
    // TODO: Implement.
    return;
}

int main(int argc, char* argv[]) {
    std::cout << "hello\n";    
    return 0;
}
