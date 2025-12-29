#pragma once
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace aZero
{
    template<size_t AlignmentType = sizeof(int32_t)>
    class LinearAllocator
    {
    public:
        static constexpr size_t Alignment = AlignmentType;

        template<typename T>
        class Handle
        {
            friend LinearAllocator;
            std::byte* m_OffsetPtr = nullptr;

            Handle(std::byte* offsetPtr)
                :m_OffsetPtr(offsetPtr) { }
        public:
            Handle() = default;
            void Write(const T& data) const { memcpy(m_OffsetPtr, &data, sizeof(T)); }
            T* operator->() { return reinterpret_cast<T*>(m_OffsetPtr); }
            T& operator*() { return *reinterpret_cast<T*>(m_OffsetPtr); }
            T& operator*() const { return *reinterpret_cast<T*>(m_OffsetPtr); }
        };

        LinearAllocator() = default;

        explicit LinearAllocator(std::byte* memoryPool, size_t capacity)
            :m_MemoryPool(memoryPool), m_OffsetPtr(memoryPool), m_Capacity(capacity) { }

        LinearAllocator(LinearAllocator&) = delete;
        LinearAllocator& operator=(LinearAllocator&) = delete;

        LinearAllocator(LinearAllocator&& other) noexcept
        {
            this->Move(other);
        }

        LinearAllocator& operator=(LinearAllocator&& other) noexcept
        {
            if (*this != other)
            {
                this->Move(other);
            }

            return *this;
        }

        template<typename T>
        [[nodiscard]] Handle<T> Allocate()
        {
            static_assert(std::is_trivially_copyable_v<T>);

            size_t freeBytes = m_Capacity - static_cast<size_t>(m_OffsetPtr - m_MemoryPool);
            void* voidOffsetPtr = static_cast<void*>(m_OffsetPtr);
            std::byte* const alignedPtr = reinterpret_cast<std::byte*>(std::align(Alignment, sizeof(T), voidOffsetPtr, freeBytes));

            if (!alignedPtr)
            {
                throw std::out_of_range("Memorypool is full");
            }

            m_OffsetPtr = alignedPtr + sizeof(T);
            return Handle<T>(alignedPtr);
        }

        template<typename T>
        [[nodiscard]] Handle<T> Append(const T& data)
        {
            const Handle<T> handle = this->Allocate<T>();
            handle.Write(data);
            return handle;
        }

        void Rewind() { m_OffsetPtr = m_MemoryPool; }
        [[nodiscard]] size_t GetCapacity() const { return m_Capacity; }
        [[nodiscard]] size_t GetOffset() const { return static_cast<size_t>(m_OffsetPtr - m_MemoryPool); }

    private:
        std::byte* m_MemoryPool = nullptr;
        std::byte* m_OffsetPtr = nullptr;
        size_t m_Capacity = 0;

        void Move(LinearAllocator& other)
        {
            m_MemoryPool = other.m_MemoryPool;
            m_OffsetPtr = other.m_OffsetPtr;
            m_Capacity = other.m_Capacity;

            other.m_MemoryPool = nullptr;
            other.m_OffsetPtr = nullptr;
            other.m_Capacity = 0;
        }
    };
}