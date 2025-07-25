#include <cstdint>
#include <stdexcept>

namespace aZero
{
    /// <summary>
    /// A thin wrapper-class that enables linear/bump allocation on an underlying memory pool.
    /// All offsets and sizes are defined in bytes.
    /// </summary>
    class LinearAllocator
    {
    public:
        /// <summary>
        /// Contains the offset and size of an allocation.
        /// </summary>
        struct Allocation
        {
            /// <summary>
            /// Allocation offset into the linear allocator.
            /// </summary>
            size_t Offset;

            /// <summary>
            /// Size of the allocation in bytes.
            /// </summary>
            size_t Size;
        };

        LinearAllocator() = default;

        /// <summary>
        /// Initializes with the input pointer and size variables.
        /// </summary>
        /// <param name="ptr">A pointer to the memory pool.</param>
        /// <param name="size">Size of the memory pool.</param>
        LinearAllocator(void* ptr, size_t size)
        {
            this->Init(ptr, size);
        }

        /// <summary>
        /// Initializes with the input pointer and size variables.
        /// </summary>
        /// <param name="ptr">A pointer to the memory pool</param>
        /// <param name="size">Size of the memory pool</param>
        void Init(void* ptr, size_t size)
        {
            m_Ptr = ptr;
            m_Size = size;
            m_Offset = 0;
        }

        /// <summary>
        /// Copies the data into the memory pool.
        /// Returns the offset into the memory pool where the data was copied.
        /// </summary>
        /// <param name="data">A pointer to the data to copy.</param>
        /// <param name="size">Size of the data to copy. Might be undefined behaviour if it's larger than the data that the pointer is referencing.</param>
        /// <returns>Offset into the memory pool where the data was copied.</returns>
        
        /// <summary>
        /// Copies the data into the memory pool.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="data">Const reference to the data that will be copied.</param>
        /// <param name="size">Size of the data in bytes.</param>
        /// <returns>An object that contains information about the completed allocation.</returns>
        template<typename T>
        Allocation Alloc(const T& data, size_t size)
        {
            const size_t sizeAfterCopy = m_Offset + size;
            if (sizeAfterCopy > m_Size)
            {
                throw std::out_of_range("Buffer is full");
            }

            const size_t allocOffset = m_Offset;
            m_Offset = sizeAfterCopy;
            memcpy((char8_t*)m_Ptr + allocOffset, (void*)&data, size);
            return { .Offset = allocOffset, .Size = size };
        }

        /// <summary>
        /// Resets the allocator to the beginning again.
        /// </summary>
        void Reset()
        {
            m_Offset = 0;
        }

        /// <summary>
        /// Gets the current offset into the memory pool.
        /// </summary>
        /// <returns>Current offset into the memory pool.</returns>
        size_t Offset() const { return m_Offset; }

        /// <summary>
        /// Gets the max size of the memory pool.
        /// </summary>
        /// <returns>Max size of the memory pool</returns>
        size_t Size() const { return m_Size; }

    private:
        /// <summary>
        /// Pointer to the memory pool.
        /// </summary>
        void* m_Ptr = nullptr;

        /// <summary>
        /// Current offset into the memory pool.
        /// </summary>
        size_t m_Offset;

        /// <summary>
        /// Max size of the memory pool. Should never exceed the memoryreferenced by m_Ptr.
        /// </summary>
        size_t m_Size;
    };
}