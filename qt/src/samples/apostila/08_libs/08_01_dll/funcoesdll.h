#ifndef FUNCOESDLL_H
#define FUNCOESDLL_H

#include "08_01_Dll_global.h"

// classe export�vel:
class DLL_08_01_DLLSHARED_EXPORT FuncoesDll
{
public:
    FuncoesDll();
	int  func(int x);
};

// fun��o export�vel:
extern "C" DLL_08_01_DLLSHARED_EXPORT int func1(int x );

// fun��o N�O-export�vel (checar vers�o do GCC)
int  func2(int x );

#endif // FUNCOESDLL_H
