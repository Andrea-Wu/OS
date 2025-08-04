#include <cmath>
#include <iostream> 
#include <optional>
#include <vector>

#include "absl/container/btree_set.h"
#include "absl/status/statusor.h"

// Here I will create a char array of size 4096, and use buddy allocation
// to allocate pointers from that array.

// The buddy allocator is an algorithm that allocates blocks in
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
// 1. Allocate 4096 chunks of size 1. 4097th chunk should fail.
// 2. Allocate 1024 chunks of size 4. 1025th chunk should fail.
// 3. Allocate 512 chunks of size 5 (which rounds up to 8)
// 4. Allocate 1 chunk of size 4096
// 5. Free all allocated pointers, then repeat test cases 1-4

// Future features:
// * Dynamically resize BuddyAllocator instead of failing to allocate pointer.

class BuddyAllocator {
 public:
    // Creates BuddyAllocator with a buffer of size 2^log_size.
    // Returns an error if the allocator cannot be created.
    static absl::StatusOr<std::unique_ptr<BuddyAllocator>> Create(int log_size);
    char* Allocate(int num_bytes);
    void Free(char* addr);
 private:
    BuddyAllocator(int log_size);
    std::vector<char> buffer_;
    // Caches log2 of the buffer size, to avoid calling std::log2() more than
    // necessary, because log implementations can be relatively expensive.
    // This should also be the size of the freelist & usedlist vectors.
    // TODO:I'm not actually sure if the std::log2 implementation is expensive.
    // The source code calls into a "built in GCC function" which is super
    // optimized by the compiler, but anyway we can experiment with this
    // later.
    const int log2_buffer_size_;
    // Stores the base pointer of the buffer. The allocator will mostly
    // operate in terms of buffer offset (for easier debug), and will 
    // convert to/from addresses only when interfacing with the client.
    char* const base_ptr_;
    // The hash map at index used_list[i] contains the address of chunks
    // of size 2^i which are in use.
    std::vector<absl::btree_set<int>> used_list_;
    // The hash map at index free_list[i] contains the address of chunks
    // of size 2^i which are not in use.
    std::vector<absl::btree_set<int>> free_list_;
};

// TODO: should validate in the Create() that log_size is valid
BuddyAllocator::BuddyAllocator(int log_size) :
    buffer_(2 << log_size, '0'),
    log2_buffer_size_(log_size),
    base_ptr_(buffer_.data()),
    used_list_(log_size),
    free_list_(log_size) {} 

absl::StatusOr<std::unique_ptr<BuddyAllocator>> Create(int log_size) {
    // TODO: implement.
    return nullptr;
}

char* BuddyAllocator::Allocate(int num_bytes) {
    // Find the smallest power of 2 greater than or equal to num_bytes.
    int pow = std::ceil(std::log2(num_bytes));
    std::cout << "pow: " << pow << "\n";
    
    // TODO: evaluate whether this edge case is necessary.
    // I think it's necessary to not exceed the vector bounds.
    if (pow > log2_buffer_size_) {
	return nullptr;
    }

    // In the simplest case, there is already a pointer available, so
    // return it and mark as used. We want to return the first pointer
    // as this seems like it would be best for reducing fragmentation.
    auto free_it = free_list_[pow].begin();
    if (free_it != free_list_[pow].end()) {
	// Check that the ptr wasn't already allocated.
	auto [used_it, emplaced] = used_list_[pow].emplace(*free_it);
	if (!emplaced) {
	    // Return nullptr to avoid allocating the same pointer twice.
	    return nullptr;
	}
	// Happy path. Return the address.
	return base_ptr_ + *free_it;
    }

    // TODO: finish implementation - need to handle the crux of this algo
    // which is splitting the 2^(`pow` + 1) memory chunk in half.
    return nullptr;
    
}

void BuddyAllocator::Free(char* addr) {
    // TODO: Implement.
    return;
}

int main(int argc, char* argv[]) {
    std::cout << "hello\n";    
    return 0;
}

