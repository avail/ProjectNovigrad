#pragma once
#include "vtable/vmthooks.h"
#include "src/DynArray.h"

namespace ProjectNovigrad
{
    namespace TW3
    {
		template<class T> class TString : TDynArray<T>;
    };
};