#include <QtCore/QCoreApplication>
#include <QThread>
#include <iostream>

/* ESTE PROJETO N�O FUNCIONA
	pois permite acesso simult�neo a um mesmo recurso compartilhado
	(o console de sa�da)

	O pr�ximo projeto corrige isso
*/

class Thread : public QThread
{
	protected:
		void run(); // virtual da base;
			// para a aplica��o � aqui que o thread come�a
	public:
		static bool doProcess;
};
bool Thread::doProcess = true;

void Thread::run()
{
	int n;
	for ( n=0; n<30 && doProcess ; ++n)
		std::cout << "thread #" << currentThreadId()
				<< " - processando: " << n << std::endl;
}
int main(int argc, char *argv[])
{
	enum { TotThreads = 4 };
	Thread threads[TotThreads];
	int n;

	for ( n=0; n<TotThreads; ++n)
		threads[n].start(); // <- inicial cada thread;

	// main: este thread continua trabalhando normalmente:

	std::cout << "threads iniciaram - tecle <enter> p/ encerrar" << std::endl;
	std::cin.get();

	// aplica��o vai encerrar (pois o "main-thread" vai encerrar):
	// ent�o, deve esperar que os demais threads encerrem
	// para que possam realizar quaisquer tarefas de finaliza��o:
	Thread::doProcess = false; // <- sinaliza para que encerrem
	for ( n=0; n<TotThreads; ++n)
		threads[n].wait(); // <- aguarda encerramento de cada um

	return 0;
}
