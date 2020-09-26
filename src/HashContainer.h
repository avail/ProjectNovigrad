#pragma once

#include <cstdint>
#include <stddef.h>
#include <Windows.h>

#include "Array.h"
#include "W3String.h"

#include "../hooking/Hooking.h"

namespace ProjectNovigrad::TW3
{
    class HashDescriptor
    {
    public:
        const wchar_t* WStr;
        const char* CStr;
        HashDescriptor* Next;
        std::uint32_t Hash;
        std::uint32_t ArrayIndex;

        __fastcall HashDescriptor(const wchar_t* wStr, const char* cStr, std::uint32_t hash);
    };

    static_assert(offsetof(HashDescriptor, WStr) == 0x00);
    static_assert(offsetof(HashDescriptor, CStr) == 0x08);
    static_assert(offsetof(HashDescriptor, Next) == 0x10);
    static_assert(offsetof(HashDescriptor, Hash) == 0x18);
    static_assert(offsetof(HashDescriptor, ArrayIndex) == 0x1C);
    static_assert(sizeof(HashDescriptor) == 0x20);

    class HashContainerEntry
    {
    public:
        std::uint64_t Field0;
        std::uint64_t Field8;
        HashContainerEntry* NextEntry;
    };

    static_assert(offsetof(HashContainerEntry, Field0) == 0x00);
    static_assert(offsetof(HashContainerEntry, Field8) == 0x08);
    static_assert(offsetof(HashContainerEntry, NextEntry) == 0x10);
    static_assert(sizeof(HashContainerEntry) == 0x18);

#pragma pack(push, 8)
    class HashContainer
    {
    public:
        CRITICAL_SECTION CritSec;
        HashDescriptor* Buckets[8191];
        HashContainerEntry EntryArray[256];
        void* UnusedDescriptorPool;
        void* UsedDescriptorPool;
        Array<HashDescriptor> Descriptors;

        static void __fastcall GetArrayIndexOrRegisterStr(std::uint32_t* out, const wchar_t* string);
        static void __fastcall GetArrayIndexOrRegisterStr(std::uint32_t* out, const W3String<wchar_t>* string);
        static const wchar_t* __fastcall GetWStrForIndex(std::uint32_t* arrayIndex);
        static const char* __fastcall GetCStrForIndex(std::uint32_t* arrayIndex);
        static std::uint32_t* __fastcall GetArrayIndex(std::uint32_t* out, std::uint32_t hash);
        static std::uint32_t* __fastcall GetHashForIndex(std::uint32_t* index, std::uint32_t* out);
    };
#pragma pack(pop)

    static_assert(offsetof(HashContainer, CritSec) == 0x00);
    static_assert(offsetof(HashContainer, Buckets) == 0x28);
    static_assert(offsetof(HashContainer, EntryArray) == 0x10020);
    static_assert(offsetof(HashContainer, UnusedDescriptorPool) == 0x11820);
    static_assert(offsetof(HashContainer, UsedDescriptorPool) == 0x11828);
    static_assert(offsetof(HashContainer, Descriptors) == 0x11830);
    static_assert(sizeof(HashContainer) == 0x11840);
}