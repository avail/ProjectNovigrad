#pragma once
// 48 89 5C 24 10 57 48 81  EC B0 00 00 00 48 8B 01

#include "../hooking/Hooking.h"
#include <iostream>

namespace ProjectNovigrad
{
  namespace TW3
  {
    class CNode;
    namespace Functions
    {

      static hook::thiscall_stub<char* (CNode*)> CNode_GetFriendlyName([]() {
        std::cout << "HOOK -> CNode_GetFriendlyName hooked!" << std::endl;
        return hook::pattern("48 89 5C 24 10 57 48 81 EC B0 00 00 00 48 8B 01")
          .count(1)
          .get(0)
          .get<void*>(0);
        });
    }
  }
}
