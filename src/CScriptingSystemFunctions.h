#pragma once

#include "hooking/Hooking.h"
#include <iostream>
#include "TString.h"

namespace ProjectNovigrad
{
    namespace TW3
    {
        class CScriptingSystem;
        namespace Functions
        {

            static hook::thiscall_stub<bool* (TString<wchar_t> const&, bool)> CScriptingSystem_CallExecFunction([]() {
                std::cout << "HOOK -> CScriptingSystem_CallExecFunction hooked!" << std::endl;
                return hook::pattern("40 55 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 83 7A ? ?")
                    .count(1)
                    .get(0)
                    .get<void*>(0);
                });
        }
    }
}
