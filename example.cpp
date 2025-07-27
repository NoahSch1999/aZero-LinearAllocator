#include "include\LinearAllocator.hpp"
using namespace aZero;

int main()
{
	// Allocate memory pool
	constexpr uint32_t memoryPoolSizeBytes = 1337;
	void* const memoryPool = new std::byte[memoryPoolSizeBytes];

	// Create linear allocator on the underlying memory pool
	LinearAllocator allocator(memoryPool, memoryPoolSizeBytes);
	// LinearAllocator<8> allocator(memoryPool, memoryPoolSizeBytes); // Ex. of 8-byte alignment

	int someInt = 42;
	double someDouble = 1337.0;

	// Allocates for an int
	auto alloc1 = allocator.Allocate(sizeof(someInt));

	// Writes using the allocation
	allocator.Write(alloc1, &someInt);

	// Allocates and writes a double
	auto alloc2 = allocator.Append(&someDouble, sizeof(someDouble));

	// Get the data from the memory pool using the allocations
	double fetchedDouble = *(double*)allocator.Get(alloc2);
	int fetchedInt = *(int*)allocator.Get(alloc1);

	// Prints "42"
	std::cout << fetchedInt << "\n";
	// Prints "1337"
	std::cout << fetchedDouble << "\n";

	return 0;
}