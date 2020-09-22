#pragma once
#include "vtable/vmthooks.h"
#include "CScriptingSystemFunctions.h"

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
        };
    };
};