#include <iostream>

#include "HashContainer.h"

#include "../hooking/Hooking.h"

namespace ProjectNovigrad::TW3
{
    #pragma region HashDescriptor
    static hook::thiscall_stub<HashDescriptor* (const wchar_t*, const char*, std::uint32_t)> HashDescriptor_Constructor([]() {
        std::cout << "HOOK -> HashDescriptor_Constructor hooked!" << std::endl;
        return hook::pattern("40 53 48 83 EC 20 33 C0 48 89 11")
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    __fastcall HashDescriptor::HashDescriptor(const wchar_t* wStr, const char* cStr, std::uint32_t hash)
        : WStr(nullptr), CStr(nullptr), Next(nullptr), Hash(0u), ArrayIndex(0u)
    {
        HashDescriptor_Constructor(wStr, cStr, hash);
    }
    #pragma endregion

    #pragma region HashContainer
    static hook::thiscall_stub<void __fastcall (std::uint32_t*, const wchar_t*)> HashContainer_GetArrayIndexOrRegisterStrWStr([]() {
        std::cout << "HOOK -> HashContainer_GetArrayIndexOrRegisterStrWStr hooked!" << std::endl;
        return hook::pattern("48 89 5C 24 08 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 74 31 66")
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    static hook::thiscall_stub<void __fastcall (std::uint32_t*, const W3String<wchar_t>*)> HashContainer_GetArrayIndexOrRegisterStrW3String([]() {
        std::cout << "HOOK -> HashContainer_GetArrayIndexOrRegisterStrW3String hooked!" << std::endl;
        return hook::pattern("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 48 8B CA 48 8B DA E8 ? 97")
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    static hook::thiscall_stub<const wchar_t* __fastcall (std::uint32_t*)> HashContainer_WStrForIndex([]() {
        std::cout << "HOOK -> HashContainer_WStrForIndex hooked!" << std::endl;
        return hook::pattern("40 53 48 83 EC 20 48 8B 05 ? ? ? 02 48 8B D9 48 85 C0 75 05 E8 ? 07")
            .count(2)
            .get(0)
            .get<void*>(0);
        });

    static hook::thiscall_stub<const char* __fastcall (std::uint32_t*)> HashContainer_GetStrForIndex([]() {
        std::cout << "HOOK -> HashContainer_GetStrForIndex hooked!" << std::endl;
        return hook::pattern("40 53 48 83 EC 20 48 8B 05 ? ? ? 02 48 8B D9 48 85 C0 75 05 E8 ? 07")
            .count(2)
            .get(1)
            .get<void*>(0);
        });

    static hook::thiscall_stub<std::uint32_t* __fastcall (std::uint32_t*, std::uint32_t)> HashContainer_GetArrayIndex([]() {
        std::cout << "HOOK -> HashContainer_GetArrayIndex hooked!" << std::endl;
        return hook::pattern("48 89 5C 24 08 57 48 83 EC 20 8B DA 48 8B F9 85 D2 74 37 81 FA 21")
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    static hook::thiscall_stub<std::uint32_t* __fastcall (std::uint32_t*, std::uint32_t*)> HashContainer_GetHashForIndex([]() {
        std::cout << "HOOK -> HashContainer_GetHashForIndex hooked!" << std::endl;
        return hook::pattern("48 89 5C 24 08 57 48 83 EC 20 48 8B 05 ? ? ? 02 48 8B DA 48 8B F9 48 85 C0 75 05 E8 ? 07")
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    
    void __fastcall HashContainer::GetArrayIndexOrRegisterStr(std::uint32_t* out, const wchar_t* string)
    {
        HashContainer_GetArrayIndexOrRegisterStrWStr(out, string);
    }

    void __fastcall HashContainer::GetArrayIndexOrRegisterStr(std::uint32_t* out, const W3String<wchar_t>* string)
    {
        HashContainer_GetArrayIndexOrRegisterStrW3String(out, string);
    }

    const wchar_t* __fastcall HashContainer::GetWStrForIndex(std::uint32_t* arrayIndex)
    {
        return HashContainer_WStrForIndex(arrayIndex);
    }

    const char* __fastcall HashContainer::GetCStrForIndex(std::uint32_t* arrayIndex)
    {
        return HashContainer_GetStrForIndex(arrayIndex);
    }

    std::uint32_t* __fastcall HashContainer::GetArrayIndex(std::uint32_t* out, std::uint32_t hash)
    {
        return HashContainer_GetArrayIndex(out, hash);
    }

    std::uint32_t* __fastcall HashContainer::GetHashForIndex(std::uint32_t* index, std::uint32_t* out)
    {
        return HashContainer_GetHashForIndex(index, out);
    }
    #pragma endregion
}
