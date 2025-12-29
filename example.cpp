#include <iostream>
#include <vector>
#include "include/LinearAllocator.hpp"

#pragma pack(push, 1)
struct MyClass
{
	char y;
	int16_t x;
	void Foo()
	{
		printf("Hello world!\n");
	}
};
#pragma pack(pop)

int main()
{
	int x = sizeof(MyClass);
	// Allocate memory pool and create a LinearAllocator referencing it
	std::vector<std::byte> pool(20);
	aZero::LinearAllocator<> allocator(pool.data(), pool.size());

	// Allocate an int and write to it using the ::Write() function
	aZero::LinearAllocator<>::Handle<int> intHandle = allocator.Allocate<int>();
	intHandle.Write(1337);

	// Allocate for a packed struct that is trivially copyable and call its function using the overloaded ->operator
	auto customClassHandle = allocator.Allocate<MyClass>();
	customClassHandle->Foo();

	// The allocator alignes the allocation since the struct was 3 bytes
	// Allocate a double and then write to it using the overloaded *operator
	auto doubleHandle = allocator.Allocate<double>();
	*doubleHandle = 42;
	std::cout << "Double handle (1): " << *doubleHandle << "\n";

	// Take a reference using the overloaded *operator
	double& doubleRef = *doubleHandle;
	doubleRef = 1337.0;
	std::cout << "Double handle (2): " << *doubleHandle << "\n";

	// Allocate and write to the allocator
	auto appendAlloc = allocator.Append(666.f);

	std::cout << "Offset before rewind: " << allocator.GetOffset() << "\n";

	// Rewind the allocator to the beginning
	allocator.Rewind();

	std::cout << "Offset after rewind: " << allocator.GetOffset() << "\n";

	std::cout << "Capacity (in bytes): " << allocator.GetCapacity() << "\n";

	return 0;
}