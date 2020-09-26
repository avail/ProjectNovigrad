#pragma once

#include <cstdint>

namespace ProjectNovigrad::TW3
{
    #pragma pack(push, 4)
    template <typename T>
    class Array
    {
    public:
        T* Data;
        std::uint32_t Length;
    };
    #pragma pack(pop)

    static_assert(offsetof(Array<char>, Data) == 0x0);
    static_assert(offsetof(Array<char>, Length) == 0x8);
    static_assert(sizeof(Array<char>) == 0xC);
}
