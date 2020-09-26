#pragma once
#include "vtable/vmthooks.h"
#include "CScriptingSystemFunctions.h"
#include "Function.h"
#include "HashContainer.h"

namespace ProjectNovigrad
{
    namespace TW3
    {
        class __declspec(dllexport) CScriptingSystem
        {
        public:
            bool CallExecFunction(char* command)
            {
                return Functions::CScriptingSystem_CallExecFunction(this, command);
            }

            void RegisterScriptFunction(const wchar_t* name, CScriptFunctionCallback callback)
            {
                std::uint32_t index;
                HashContainer::GetArrayIndexOrRegisterStr(&index, name);

                CFunction::Create(mem, &index, callback);
            }
        };
    };
};