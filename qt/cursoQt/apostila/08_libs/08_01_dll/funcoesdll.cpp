#include "funcoesdll.h"


FuncoesDll::FuncoesDll()
{
}
int FuncoesDll::func(int x)
{
	return x + 1;
}

// fun��o export�vel:
extern "C" DLL_08_01_DLLSHARED_EXPORT int func1(int x )
{
	return x + 2;
}

// fun��o N�O-export�vel (checar vers�o do GCC)
int func2(int x )
{
	return x + 3;
}

