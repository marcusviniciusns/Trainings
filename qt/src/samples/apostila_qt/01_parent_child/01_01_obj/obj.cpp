
#include <QObject>
#include <iostream>

int main()
{
	QObject * pobj_1 = new QObject(0);		// "pobj_1" n�o tem "parent"
	QObject * pobj_2 = new QObject(pobj_1); // "pobj_1" � o "parent" de "pobj_2"
	QObject * pobj_3 = new QObject(pobj_2); // "pobj_2" � o "parent" de "pobj_3"
	
	delete pobj_1;
	// - em consequ�ncia:
	std::cout <<"1. a destrutora de 'pobj_1' deletou 'pobj_2'\n";
	std::cout <<"2. a destrutora de 'pobj_2' deletou 'pboj_3'\n";

	return 0;
}


