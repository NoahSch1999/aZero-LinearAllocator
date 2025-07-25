#include "include\LinearAllocator.hpp"
using namespace aZero;

int main()
{
	// Allocate memory pool
	constexpr uint32_t memoryPoolSizeBytes = 1337;
	void* const memoryPool = new char8_t[memoryPoolSizeBytes];

	// Create linear allocator on the underlying memory pool
	LinearAllocator allocator(memoryPool, memoryPoolSizeBytes);

	// Copying a std::string into the linear allocator
	const std::string str("Hello");
	LinearAllocator::Allocation alloc1 = allocator.Alloc(str, str.size());

	// Copying an int into the linear allocator
	const uint32_t someInt = 42;
	LinearAllocator::Allocation alloc2 = allocator.Alloc(someInt, sizeof(someInt));

	// Copying an int from the underlying memory pool using the information returned on allocation
	uint32_t fetchedInt;
	memcpy((void*)&fetchedInt, (char8_t*)memoryPool + alloc2.Offset, alloc2.Size);

	// Prints "42"
	std::cout << fetchedInt << "\n";

	return 0;
}