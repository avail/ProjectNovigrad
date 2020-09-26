#pragma once

#include <cstdint>
#include <stddef.h>

namespace ProjectNovigrad::TW3
{
    class CScriptStackFrame
    {
    public:
        std::uint64_t Padding1[3];
        CFunction* Function;
        std::uint64_t Padding2;
        std::uint64_t Param;
        std::uint8_t* Data;
        std::uint64_t Padding3[29];
    };

    static_assert(offsetof(CScriptStackFrame, Function) == 0x18);
    static_assert(offsetof(CScriptStackFrame, Param) == 0x28);
    static_assert(offsetof(CScriptStackFrame, Data) == 0x30);
    static_assert(sizeof(CScriptStackFrame) == 0x120);

    class CFunction;

    class CFunctionVtbl
    {
    public:
        void (__fastcall* VirtualDestructor)(CFunction*, char);
    };

    class CFunction
    {
    public:
        using CallbackType = void(__fastcall*)(void*, CScriptStackFrame*, void*);

        CFunctionVtbl* vfptr;
        std::uint64_t Field8;
        std::uint32_t ArrayIndex;
        std::uint32_t Flags;
        std::uint64_t Padding[18];
        std::uint32_t HandlerIndex;
        std::uint32_t Padding2[5];

        static CFunction* Create(std::uint32_t* a2, void* cb);
        static void Delete(CFunction* instance);
    };

    static_assert(offsetof(CFunction, vfptr) == 0x00);
    static_assert(offsetof(CFunction, Field8) == 0x08);
    static_assert(offsetof(CFunction, ArrayIndex) == 0x10);
    static_assert(offsetof(CFunction, Flags) == 0x14);
    static_assert(offsetof(CFunction, HandlerIndex) == 0xA8);
    static_assert(sizeof(CFunction) == 0xC0);
}