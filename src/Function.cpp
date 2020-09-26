#include <iostream>

#include "Function.h"

#include "../hooking/Hooking.h"

namespace ProjectNovigrad::TW3
{
    static hook::thiscall_stub<CFunction* __fastcall(const wchar_t*, const char*, std::uint32_t)> CFunction_Ctor([]() {
        std::cout << "HOOK -> CFunction_Ctor hooked!" << std::endl;
        return hook::pattern("") // TODO
            .count(1)
            .get(0)
            .get<void*>(0);
        });

    CFunction* CFunction::Create(std::uint32_t* a2, void* cb)
    {
        // TODO
    }
}