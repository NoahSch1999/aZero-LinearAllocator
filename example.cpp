#include <iostream>
#include <vector>
#include "../LinearAllocator.hpp"

struct MyClass
{
	int x;
	void Foo()
	{
		printf("Hello world!\n");
	}
};

int main()
{
	// Allocate memory pool and create a LinearAllocator referencing it
	std::vector<std::byte> pool(20);
	aZero::LinearAllocator<> allocator(pool.data(), pool.size());

	// Allocate an int and write to it using the ::Write() function
	aZero::LinearAllocator<>::Handle<int> intHandle = allocator.Allocate<int>();
	intHandle.Write(1337);

	// Allocate a double and then write to it using the overloaded *operator
	auto doubleHandle = allocator.Allocate<double>();
	*doubleHandle = 42;
	std::cout << "Double handle (1): " << *doubleHandle << "\n";

	// Take a reference using the overloaded *operator
	double& doubleRef = *doubleHandle;
	doubleRef = 1337.0;
	std::cout << "Double handle (2): " << *doubleHandle << "\n";

	// Allocate for a custom trivially copyable class and call its function using the overloaded ->operator
	auto customClassHandle = allocator.Allocate<MyClass>();
	customClassHandle->Foo();

	// Allocate and write to the allocator
	auto appendAlloc = allocator.Append(666.f);

	std::cout << "Offset before rewind: " << allocator.GetOffset() << "\n";

	// Rewind the allocator to the beginning
	allocator.Rewind();

	std::cout << "Offset after rewind: " << allocator.GetOffset() << "\n";

	std::cout << "Capacity (in bytes): " << allocator.GetCapacity() << "\n";

	return 0;
}