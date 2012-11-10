#include <QtCore/QCoreApplication>
#include <iostream>
#include "../08_01_Dll/funcoesdll.h"
#include <QLibrary>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	 // linkagem IMPL�CITA (usa a implib declarada no arquivo ".pro")

	 // classe C++
	 FuncoesDll fdll;
	std::cout << fdll.func( 1 ) << "\n";

	// fun��o global:
	std::cout << func1(2) << "\n";

	// linkagem EXPL�CITA:

	QLibrary lib;
	lib.setFileName("c:\\cursoQt\\08_01_Dll.dll");
	lib.load();
	typedef int ( *func1_t)(int);
	func1_t func1_p = (func1_t)lib.resolve("func1");
	std::cout << func1_p(3);
	lib.unload();

	// a linha abaixo d� de erro de linker
	// pois "func2" n�o � exportada pela DLL:
//	std::cout << func2(3) << "\n";

	std::cin.get();
}
