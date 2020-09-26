#pragma once

#include "Array.h"

#include <stddef.h>

namespace ProjectNovigrad::TW3
{
    #pragma pack(push, 4)
    template <typename T>
    class W3String : public Array<T>
    {
    public:
    };
    #pragma pack(pop)

    template class W3String<char>;
    template class W3String<wchar_t>;

    static_assert(offsetof(W3String<char>, Data) == 0x0);
    static_assert(offsetof(W3String<char>, Length) == 0x8);
    static_assert(sizeof(W3String<char>) == 0xC);
}
