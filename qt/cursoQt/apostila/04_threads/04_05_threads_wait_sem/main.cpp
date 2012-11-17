/*


  Este exemplo d� conta de uma situa��o
  em que um thread precise esperar 
  qualquer ponto de processamento de outro thread
  que n�o seja a finaliza��o desse thread.

  Para isso pode ser usado um sem�foro
  com seu contador iniciado com zero.

  Neste exemplo, "main" ir� esperar
  que cada "TestThread" j� tenha iniciado.

*/


#include <QtCore>

#include <iostream>

static QSemaphore sem_mainWait( 0 ); // contador inicial: zero
							// ali�s, zero j� � o default

static QMutex mut_cout;

bool g_bTestThreadsProcess;

class TestThread : public QThread
{
	int _m_nThreadNumber;

	public:
		void  setNumber( int nThreadNumber ) { _m_nThreadNumber = nThreadNumber; }

	protected: 
		void run(); // redefine virtual da base
} ; 

void TestThread::run()
{
	// executa qualquer tarefa de inicializa��o
	// da qual o 'main-thread' dependa.
	
	// agora j� pode liberar o main-thread 
	// que est� aguardando at� que este thread tenha iniciado:

	sem_mainWait.release(1); // incrementa sem�foro liberando main

	mut_cout.lock();
		std::cout << "*** TestThread(#" << _m_nThreadNumber << ")::run: liberei o main thread" << std::endl;
	mut_cout.unlock();
		
	int n;
	for ( n=0; n<100 && g_bTestThreadsProcess; ++n )
	{
		mut_cout.lock();
			std::cout << "TestThread(#" << _m_nThreadNumber << ")::run: agora vou trabalhar... " << n << std::endl;
		mut_cout.unlock();
	
		usleep( 20 ); // simula uma tarefa a executar
			// com dura��o de 20 MICRO-segundos.
	}

	mut_cout.lock();
		std::cout << "*** TestThread(#" << _m_nThreadNumber << ")::run: vou encerrar." << std::endl;
	mut_cout.unlock();
}

int main()
{
	g_bTestThreadsProcess = true;

	enum { TOTAL_TEST_THREADS = 3};
	TestThread testThreads[ TOTAL_TEST_THREADS ];

	int nTh;

	// garante que "main" conseguir� imprimir antes dos "TestThreads":
	mut_cout.lock();

	for ( nTh = 0 ; nTh < TOTAL_TEST_THREADS ; ++nTh )
	{

		testThreads[ nTh ].setNumber( nTh+1 );
		testThreads[ nTh ].start();

		// 'main' espera que cada inst�ncia de 'TestThread' inicie:
		sem_mainWait.acquire( 1 );
	}

		std::cout << "\n* main: as 3 instancias de TestThread iniciaram" << std::endl; 
		std::cout << "* main: tecle <enter> para encerrar" << std::endl;
	mut_cout.unlock();

	std::cin.get();

	mut_cout.lock();
		std::cout << "\n* main: agora vou fazer com que encerrem." << std::endl;
	mut_cout.unlock();

	g_bTestThreadsProcess = false; // posiciona o flag para encerramento

	// aguarda o encerramento dos threads:
	for ( nTh = 0 ; nTh < TOTAL_TEST_THREADS ; ++nTh )
		testThreads[ nTh ].wait();

	std::cout << "\n* main: threads encerraram; agora, aplicacao vai encerar.\nFIM" << std::endl;
	return 0;
}


